#include "ui/Editor.h"

#include <ncurses.h>

#include "concurrent/Snapshot.h"
#include "io/FileIO.h"
#include "spell/SpellChecker.h"
#include "spell/Suggester.h"
#include "ui/Prompt.h"
#include "ui/StatusBar.h"

namespace editor {

using namespace std::chrono_literals;

Editor::Editor(std::string initialFile) : initialFile_(std::move(initialFile)) {}

void Editor::run() {
    timeout(16); // ~60fps poll: getch() returns ERR instead of blocking, so
                 // the loop can drain worker events between keystrokes.

    // Async dictionary load: the editor is interactive immediately, and
    // spell features light up via dictReady_ once this completes. The
    // std::atomic store/load pair below is what makes handing an
    // unordered_set built entirely on a worker thread over to the main
    // thread safe without a mutex - the dictionary is never mutated again
    // after dictReady_ is set.
    pool_.submit([this] {
        dictionary_.loadFromFile("dictionary.txt");
        dictReady_ = true;
        events_.push(DictionaryLoadedEvent{dictionary_.size()});
        return 0;
    });

    if (!initialFile_.empty()) {
        std::string path = initialFile_;
        pool_.submit([this, path] {
            LoadResult r = loadFile(path);
            events_.push(LoadCompleteEvent{r.success, r.lines, path, r.error, r.trailingNewline});
            return 0;
        });
    }

    lastEditTime_ = std::chrono::steady_clock::now();
    lastAutosave_ = lastEditTime_;
    auto lastDraw = std::chrono::steady_clock::now();

    while (running_) {
        int ch = getch();
        bool acted = false;
        if (ch != ERR) {
            handleKey(ch);
            acted = true;
        }
        if (processEvents()) acted = true;
        maybeTriggerScan();
        maybeAutosave();

        auto now = std::chrono::steady_clock::now();
        if (acted || now - lastDraw > 200ms) {
            draw();
            lastDraw = now;
        }
    }
}

void Editor::markEdited() {
    lastEditTime_ = std::chrono::steady_clock::now();
    docVersion_++;
    scanPending_ = true;
}

void Editor::cancelPendingSuggestion() {
    if (suggestCancelFlag_) suggestCancelFlag_->store(true);
}

void Editor::requestSuggestions() {
    if (!dictReady_) {
        statusMessage_ = "Dictionary still loading...";
        return;
    }
    auto word = doc_.buffer().wordAt(doc_.buffer().cursor());
    if (word.text.empty()) {
        statusMessage_ = "No word at cursor.";
        return;
    }

    cancelPendingSuggestion(); // tell any in-flight request to stop
    auto myFlag = std::make_shared<std::atomic<bool>>(false);
    suggestCancelFlag_ = myFlag;
    int myVersion = ++suggestVersion_;
    std::string w = word.text;

    pool_.submit([this, w, myVersion, myFlag] {
        auto result = suggest(dictionary_, w, *myFlag);
        events_.push(SuggestEvent{myVersion, std::move(result)});
        return 0;
    });
}

void Editor::handleKey(int ch) {
    switch (ch) {
        case 27: // ESC: quit
            if (confirmQuitIfDirty()) running_ = false;
            return;

        case KEY_LEFT: cancelPendingSuggestion(); doc_.buffer().moveLeft(); break;
        case KEY_RIGHT: cancelPendingSuggestion(); doc_.buffer().moveRight(); break;
        case KEY_UP: cancelPendingSuggestion(); doc_.buffer().moveUp(); break;
        case KEY_DOWN: cancelPendingSuggestion(); doc_.buffer().moveDown(); break;
        case KEY_HOME: cancelPendingSuggestion(); doc_.buffer().moveHome(); break;
        case KEY_END: cancelPendingSuggestion(); doc_.buffer().moveEnd(); break;
        case KEY_PPAGE: cancelPendingSuggestion(); doc_.buffer().movePageUp(viewportRows()); break;
        case KEY_NPAGE: cancelPendingSuggestion(); doc_.buffer().movePageDown(viewportRows()); break;

        case KEY_ENTER: case '\n': case '\r':
            doc_.newline();
            markEdited();
            break;

        case KEY_BACKSPACE: case 127: case 8:
            doc_.backspace();
            markEdited();
            break;

        case KEY_DC:
            doc_.deleteForward();
            markEdited();
            break;

        case 1: // Ctrl+A: toggle selection mode; arrow keys extend it
            if (doc_.hasSelection()) doc_.clearSelection();
            else doc_.startSelection();
            break;

        case 11: // Ctrl+K: copy
            doc_.copySelection(clipboard_);
            statusMessage_ = clipboard_.empty() ? "Nothing selected." : "Copied.";
            break;

        case 24: // Ctrl+X: cut
            if (doc_.hasSelection()) {
                doc_.cutSelection(clipboard_);
                markEdited();
                statusMessage_ = "Cut.";
            }
            break;

        case 22: // Ctrl+V: paste
            doc_.pasteFrom(clipboard_);
            markEdited();
            break;

        case 26: // Ctrl+Z: undo
            statusMessage_ = doc_.undo() ? "Undo." : "Nothing to undo.";
            markEdited();
            break;

        case 25: // Ctrl+Y: redo
            statusMessage_ = doc_.redo() ? "Redo." : "Nothing to redo.";
            markEdited();
            break;

        case 23: // Ctrl+W: suggestions for word at cursor
            requestSuggestions();
            break;

        case 12: doLoad(); break;      // Ctrl+L
        case 18: doSave(false); break; // Ctrl+R: save (remembered name, or prompt once)
        case 4:  doSave(true); break;  // Ctrl+D: save as
        case 6:  doFind(); break;      // Ctrl+F
        case 5:  doReplace(); break;   // Ctrl+E: find & replace all

        case KEY_RESIZE:
            clear();
            break;

        default:
            if (ch >= 32 && ch <= 126) {
                if (doc_.hasSelection()) doc_.deleteSelection();
                doc_.typeChar(static_cast<char>(ch));
                markEdited();
            }
            break;
    }
}

bool Editor::processEvents() {
    auto drained = events_.drainAll();
    for (auto& ev : drained) {
        std::visit([this](auto&& e) { this->onEvent(e); }, ev);
    }
    return !drained.empty();
}

void Editor::onEvent(const DictionaryLoadedEvent& e) {
    statusMessage_ = "Dictionary loaded: " + std::to_string(e.wordCount) + " words.";
    scanPending_ = true; // now that the dictionary is ready, scan what's loaded so far
}

void Editor::onEvent(const SpellScanEvent& e) {
    if (e.version != docVersion_.load()) return; // stale - buffer changed since this scan started
    misspellings_ = e.spans;
}

void Editor::onEvent(const SuggestEvent& e) {
    if (e.version != suggestVersion_) return; // stale - superseded by a newer request
    lastSuggestions_ = e.result;
}

void Editor::onEvent(const SaveCompleteEvent& e) {
    if (e.success) {
        statusMessage_ = "Saved to '" + e.path + "'.";
        doc_.setFilename(e.path);
        doc_.markClean();
    } else {
        statusMessage_ = e.error;
    }
}

void Editor::onEvent(const LoadCompleteEvent& e) {
    if (e.success) {
        doc_.loadLines(e.lines);
        doc_.setFilename(e.path);
        doc_.setTrailingNewline(e.trailingNewline);
        view_.topLine = 0;
        statusMessage_ = "Loaded '" + e.path + "'.";
        markEdited();
        misspellings_.clear();
    } else {
        statusMessage_ = e.error;
    }
}

void Editor::maybeTriggerScan() {
    if (!scanPending_ || !dictReady_) return;
    auto now = std::chrono::steady_clock::now();
    if (now - lastEditTime_ < 150ms) return; // debounce: wait for a pause in typing
    scanPending_ = false;

    int version = docVersion_.load();
    BufferSnapshot snapshot = makeSnapshot(doc_.buffer().lines());
    pool_.submit([this, version, snapshot] {
        std::atomic<bool> neverCancel{false}; // scan staleness is handled by version, not cancellation
        auto spans = scanBuffer(*snapshot, dictionary_, neverCancel);
        events_.push(SpellScanEvent{version, std::move(spans)});
        return 0;
    });
}

void Editor::maybeAutosave() {
    if (!doc_.dirty() || !doc_.hasFilename()) return;
    auto now = std::chrono::steady_clock::now();
    if (now - lastEditTime_ < 3s) return;  // wait for a pause in typing
    if (now - lastAutosave_ < 5s) return;  // don't spam saves
    lastAutosave_ = now;

    std::string path = doc_.filename();
    bool trailingNewline = doc_.trailingNewline();
    BufferSnapshot snapshot = makeSnapshot(doc_.buffer().lines());
    pool_.submit([this, path, snapshot, trailingNewline] {
        SaveResult r = saveFile(path, *snapshot, trailingNewline);
        events_.push(SaveCompleteEvent{r.success, path, r.error});
        return 0;
    });
}

void Editor::draw() {
    ensureCursorVisible(doc_, view_, viewportRows());

    move(0, 0);
    clrtoeol();
    printw("ESC quit | ^L load ^R save ^D save-as | ^A select ^K copy ^X cut ^V paste | ^Z undo ^Y redo | ^F find ^E replace | ^W suggest");

    drawStatusBar(1, COLS, doc_, dictReady_, dictionary_.size(), statusMessage_);

    move(2, 0);
    clrtoeol();
    if (!lastSuggestions_.word.empty()) {
        if (lastSuggestions_.correct) {
            printw("'%s' is spelled correctly.", lastSuggestions_.word.c_str());
        } else {
            printw("Suggestions for '%s': ", lastSuggestions_.word.c_str());
            for (auto& s : lastSuggestions_.suggestions) printw("%s ", s.c_str());
            if (lastSuggestions_.suggestions.empty()) printw("(none found)");
        }
    }

    renderBuffer(doc_, view_, viewportRows(), COLS, misspellings_);
    refresh();
}

int Editor::viewportRows() const {
    return LINES - kHeaderRows;
}

bool Editor::confirmQuitIfDirty() {
    if (!doc_.dirty()) return true;
    return promptYesNo(0, "Unsaved changes. Quit anyway?");
}

void Editor::doSave(bool saveAs) {
    std::string path = doc_.filename();
    if (saveAs || !doc_.hasFilename()) {
        std::string entered = promptInput(0, "Save as: ");
        if (entered.empty()) {
            statusMessage_ = "Save cancelled.";
            return;
        }
        path = entered;
    }

    statusMessage_ = "Saving...";
    bool trailingNewline = doc_.trailingNewline();
    BufferSnapshot snapshot = makeSnapshot(doc_.buffer().lines());
    pool_.submit([this, path, snapshot, trailingNewline] {
        SaveResult r = saveFile(path, *snapshot, trailingNewline);
        events_.push(SaveCompleteEvent{r.success, path, r.error});
        return 0;
    });
}

void Editor::doLoad() {
    std::string entered = promptInput(0, "Load file: ");
    if (entered.empty()) {
        statusMessage_ = "Load cancelled.";
        return;
    }

    std::string path = entered;
    statusMessage_ = "Loading...";
    pool_.submit([this, path] {
        LoadResult r = loadFile(path);
        events_.push(LoadCompleteEvent{r.success, r.lines, path, r.error, r.trailingNewline});
        return 0;
    });
}

void Editor::doFind() {
    std::string needle = promptInput(0, "Find: ");
    if (needle.empty()) return;
    lastSearch_ = needle;
    statusMessage_ = doc_.findNext(needle, false) ? "Found." : ("'" + needle + "' not found.");
}

void Editor::doReplace() {
    std::string needle = promptInput(0, "Replace - find: ");
    if (needle.empty()) return;
    std::string replacement = promptInput(0, "Replace with: ");

    int count = doc_.replaceAll(needle, replacement, false);
    statusMessage_ = std::to_string(count) + " replacement(s).";
    if (count > 0) markEdited();
}

} // namespace editor
