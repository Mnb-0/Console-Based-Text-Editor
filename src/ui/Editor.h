#pragma once
#include <atomic>
#include <chrono>
#include <memory>
#include <string>

#include "concurrent/EventQueue.h"
#include "concurrent/ThreadPool.h"
#include "core/Clipboard.h"
#include "core/Document.h"
#include "spell/Dictionary.h"
#include "spell/Suggester.h"
#include "ui/Renderer.h"
#include "ui/Screen.h"

namespace editor {

class Editor {
public:
    explicit Editor(std::string initialFile);
    void run();

private:
    // Member declaration order doubles as shutdown order: members are
    // destroyed in REVERSE declaration order, so pool_ (declared last) is
    // destroyed FIRST. That joins every worker thread - which may still be
    // reading dictionary_ or pushing to events_ - before those objects are
    // torn down, and screen_ (declared first, destroyed last) keeps the
    // terminal alive until everything else is gone. Do not reorder these
    // without re-checking that invariant.
    Screen screen_;
    EventQueue events_;
    Dictionary dictionary_;
    Document doc_;
    Clipboard clipboard_;
    ViewState view_;

    std::atomic<bool> dictReady_{false};
    std::atomic<int> docVersion_{0};
    bool scanPending_ = false;
    std::vector<MisspelledSpan> misspellings_;
    SuggestionResult lastSuggestions_;
    int suggestVersion_ = 0;
    std::shared_ptr<std::atomic<bool>> suggestCancelFlag_;

    std::string initialFile_;
    std::string statusMessage_;
    std::string lastSearch_;
    bool running_ = true;

    std::chrono::steady_clock::time_point lastEditTime_;
    std::chrono::steady_clock::time_point lastAutosave_;

    ThreadPool pool_;

    void handleKey(int ch);
    bool processEvents();
    void onEvent(const DictionaryLoadedEvent&);
    void onEvent(const SpellScanEvent&);
    void onEvent(const SuggestEvent&);
    void onEvent(const SaveCompleteEvent&);
    void onEvent(const LoadCompleteEvent&);

    void markEdited();
    void cancelPendingSuggestion();
    void requestSuggestions();
    void maybeTriggerScan();
    void maybeAutosave();
    void draw();
    int viewportRows() const;
    bool confirmQuitIfDirty();
    void doSave(bool saveAs);
    void doLoad();
    void doFind();
    void doReplace();
};

} // namespace editor
