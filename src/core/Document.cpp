#include "core/Document.h"

#include <algorithm>
#include <cctype>
#include <utility>

namespace editor {

void Document::typeChar(char c) {
    Position start = buffer_.cursor();
    std::string ins(1, c);
    buffer_.insertText(start, ins);
    undo_.record(start, "", ins);
}

void Document::newline() {
    Position start = buffer_.cursor();
    buffer_.insertText(start, "\n");
    undo_.record(start, "", "\n");
}

void Document::backspace() {
    Position cur = buffer_.cursor();
    if (cur.row == 0 && cur.col == 0) return;

    Position from = (cur.col > 0)
        ? Position{cur.row, cur.col - 1}
        : Position{cur.row - 1, static_cast<int>(buffer_.lines()[cur.row - 1].size())};

    std::string removed = buffer_.eraseRange(from, cur);
    undo_.record(from, removed, "");
}

void Document::deleteForward() {
    Position cur = buffer_.cursor();
    Position to;
    if (cur.col < static_cast<int>(buffer_.lines()[cur.row].size())) {
        to = {cur.row, cur.col + 1};
    } else if (cur.row + 1 < buffer_.lineCount()) {
        to = {cur.row + 1, 0};
    } else {
        return;
    }

    std::string removed = buffer_.eraseRange(cur, to);
    undo_.record(cur, removed, "");
}

void Document::startSelection() {
    selecting_ = true;
    selectionAnchor_ = buffer_.cursor();
}

void Document::clearSelection() {
    selecting_ = false;
}

std::pair<Position, Position> Document::selectionRange() const {
    Position a = selectionAnchor_, b = buffer_.cursor();
    if (b < a) std::swap(a, b);
    return {a, b};
}

void Document::copySelection(Clipboard& clip) const {
    if (!selecting_) return;
    auto [a, b] = selectionRange();
    clip.set(buffer_.textInRange(a, b));
}

void Document::cutSelection(Clipboard& clip) {
    if (!selecting_) return;
    auto [a, b] = selectionRange();
    std::string removed = buffer_.eraseRange(a, b);
    undo_.record(a, removed, "");
    clip.set(removed);
    selecting_ = false;
}

void Document::deleteSelection() {
    if (!selecting_) return;
    auto [a, b] = selectionRange();
    std::string removed = buffer_.eraseRange(a, b);
    undo_.record(a, removed, "");
    selecting_ = false;
}

void Document::pasteFrom(const Clipboard& clip) {
    if (clip.empty()) return;
    if (selecting_) deleteSelection();
    Position start = buffer_.cursor();
    buffer_.insertText(start, clip.get());
    undo_.record(start, "", clip.get());
}

bool Document::undo() { return undo_.undo(buffer_); }
bool Document::redo() { return undo_.redo(buffer_); }

void Document::selectMatch(Position start, int len) {
    selectionAnchor_ = start;
    buffer_.setCursor({start.row, start.col + len});
    selecting_ = true;
}

namespace {
std::string toLowerCopy(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); });
    return s;
}
} // namespace

bool Document::findNext(const std::string& needle, bool caseSensitive) {
    if (needle.empty()) return false;
    std::string target = caseSensitive ? needle : toLowerCopy(needle);
    Position cur = buffer_.cursor();
    int rows = buffer_.lineCount();

    auto searchRow = [&](int r, size_t fromCol, size_t toCol) -> long {
        std::string hay = buffer_.lines()[r];
        if (!caseSensitive) hay = toLowerCopy(hay);
        if (toCol < hay.size()) hay = hay.substr(0, toCol);
        if (fromCol > hay.size()) return -1;
        size_t pos = hay.find(target, fromCol);
        return pos == std::string::npos ? -1 : static_cast<long>(pos);
    };

    // Phase 1: from just after the cursor to the end of the buffer.
    for (int r = cur.row; r < rows; ++r) {
        size_t fromCol = (r == cur.row) ? static_cast<size_t>(cur.col + 1) : 0;
        long pos = searchRow(r, fromCol, std::string::npos);
        if (pos >= 0) { selectMatch({r, static_cast<int>(pos)}, static_cast<int>(target.size())); return true; }
    }
    // Phase 2: wrap around, from the start of the buffer up to the cursor.
    for (int r = 0; r <= cur.row; ++r) {
        size_t toCol = (r == cur.row) ? static_cast<size_t>(cur.col + 1) : std::string::npos;
        long pos = searchRow(r, 0, toCol);
        if (pos >= 0) { selectMatch({r, static_cast<int>(pos)}, static_cast<int>(target.size())); return true; }
    }
    return false;
}

int Document::replaceAll(const std::string& needle, const std::string& replacement, bool caseSensitive) {
    if (needle.empty()) return 0;
    std::string target = caseSensitive ? needle : toLowerCopy(needle);
    int count = 0;
    Position pos{0, 0};

    while (pos.row < buffer_.lineCount()) {
        std::string hay = buffer_.lines()[pos.row];
        if (!caseSensitive) hay = toLowerCopy(hay);
        size_t idx = hay.find(target, pos.col);
        if (idx == std::string::npos) {
            pos = {pos.row + 1, 0};
            continue;
        }
        Position start{pos.row, static_cast<int>(idx)};
        Position end{pos.row, static_cast<int>(idx + target.size())};
        std::string removed = buffer_.eraseRange(start, end);
        buffer_.insertText(start, replacement);
        undo_.record(start, removed, replacement);
        count++;
        pos = {start.row, start.col + static_cast<int>(replacement.size())};
    }
    return count;
}

void Document::loadLines(std::vector<std::string> lines) {
    buffer_.loadLines(std::move(lines));
    undo_ = UndoStack{};
    selecting_ = false;
}

} // namespace editor
