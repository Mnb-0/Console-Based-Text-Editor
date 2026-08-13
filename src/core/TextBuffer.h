#pragma once
#include <string>
#include <vector>

namespace editor {

struct Position {
    int row = 0;
    int col = 0;
};

inline bool operator==(const Position& a, const Position& b) { return a.row == b.row && a.col == b.col; }
inline bool operator!=(const Position& a, const Position& b) { return !(a == b); }
inline bool operator<(const Position& a, const Position& b) {
    return a.row != b.row ? a.row < b.row : a.col < b.col;
}

struct WordSpan {
    std::string text;
    Position start;
    Position end;
};

// Advances `start` by `text`, treating '\n' as a line break. Pure string math,
// used to compute the end position of an insertion without touching a buffer -
// shared by TextBuffer and UndoStack.
Position advance(Position start, const std::string& text);

// Line-oriented text storage: a vector of lines plus a (row, col) cursor.
// Mutations go through insertText/eraseRange so that Document can record a
// single undo entry per edit regardless of whether it spans lines.
class TextBuffer {
public:
    TextBuffer();

    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void moveHome();
    void moveEnd();
    void movePageUp(int pageSize);
    void movePageDown(int pageSize);
    void setCursor(Position p);
    Position cursor() const { return cursor_; }

    // Inserts `text` (may contain '\n') at `at`, moves the cursor to the end
    // of the inserted text, and returns that end position.
    Position insertText(Position at, const std::string& text);

    // Erases [from, to) (order-independent - swapped if needed), moves the
    // cursor to the start of the range, and returns the erased text.
    std::string eraseRange(Position from, Position to);

    // Read-only equivalent of eraseRange, used for copy and search.
    std::string textInRange(Position from, Position to) const;

    WordSpan wordAt(Position p) const;
    Position clampPosition(Position p) const;

    void loadLines(std::vector<std::string> lines);
    const std::vector<std::string>& lines() const { return lines_; }

    int lineCount() const { return static_cast<int>(lines_.size()); }
    bool modified() const { return modified_; }
    void clearModified() { modified_ = false; }

private:
    std::vector<std::string> lines_;
    Position cursor_;
    int desiredCol_ = 0;
    bool modified_ = false;
};

} // namespace editor
