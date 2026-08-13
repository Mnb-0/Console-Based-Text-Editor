#pragma once
#include <vector>

#include "core/EditOp.h"
#include "core/TextBuffer.h"

namespace editor {

// Operation-stack undo/redo. Consecutive single-character, non-newline
// inserts are coalesced into one EditOp so a single undo removes a whole
// run of typing rather than one letter.
class UndoStack {
public:
    void record(Position start, std::string removed, std::string inserted);
    bool undo(TextBuffer& buf);
    bool redo(TextBuffer& buf);
    bool canUndo() const { return !undoOps_.empty(); }
    bool canRedo() const { return !redoOps_.empty(); }

private:
    std::vector<EditOp> undoOps_;
    std::vector<EditOp> redoOps_;
    Position lastEnd_{};
    bool hasLastEnd_ = false;
};

} // namespace editor
