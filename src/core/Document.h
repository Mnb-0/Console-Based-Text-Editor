#pragma once
#include <string>
#include <utility>
#include <vector>

#include "core/Clipboard.h"
#include "core/TextBuffer.h"
#include "core/UndoStack.h"

namespace editor {

// Owns the buffer plus everything that turns raw text edits into a usable
// editor: undo, selection, clipboard transfer, find/replace, and file
// identity (name + dirty flag).
class Document {
public:
    Document() = default;

    TextBuffer& buffer() { return buffer_; }
    const TextBuffer& buffer() const { return buffer_; }

    void typeChar(char c);
    void newline();
    void backspace();
    void deleteForward();

    void startSelection();
    void clearSelection();
    bool hasSelection() const { return selecting_; }
    std::pair<Position, Position> selectionRange() const;
    void copySelection(Clipboard& clip) const;
    void cutSelection(Clipboard& clip);
    void deleteSelection();
    void pasteFrom(const Clipboard& clip);

    bool undo();
    bool redo();

    bool findNext(const std::string& needle, bool caseSensitive);
    int replaceAll(const std::string& needle, const std::string& replacement, bool caseSensitive);

    void setFilename(const std::string& name) { filename_ = name; hasFilename_ = true; }
    bool hasFilename() const { return hasFilename_; }
    const std::string& filename() const { return filename_; }

    void setTrailingNewline(bool v) { trailingNewline_ = v; }
    bool trailingNewline() const { return trailingNewline_; }

    bool dirty() const { return buffer_.modified(); }
    void markClean() { buffer_.clearModified(); }

    void loadLines(std::vector<std::string> lines);

private:
    TextBuffer buffer_;
    UndoStack undo_;
    bool selecting_ = false;
    Position selectionAnchor_{};
    std::string filename_;
    bool hasFilename_ = false;
    bool trailingNewline_ = true;

    void selectMatch(Position start, int len);
};

} // namespace editor
