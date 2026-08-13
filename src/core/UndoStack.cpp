#include "core/UndoStack.h"

#include <utility>

namespace editor {

void UndoStack::record(Position start, std::string removed, std::string inserted) {
    redoOps_.clear();

    bool simpleInsert = removed.empty() && inserted.size() == 1 && inserted[0] != '\n';
    if (simpleInsert && !undoOps_.empty() && hasLastEnd_ && lastEnd_ == start) {
        EditOp& back = undoOps_.back();
        if (back.removed.empty() && !back.inserted.empty() && back.inserted.back() != '\n') {
            back.inserted += inserted;
            lastEnd_ = advance(start, inserted);
            return;
        }
    }

    Position end = advance(start, inserted);
    undoOps_.push_back(EditOp{start, std::move(removed), std::move(inserted)});
    lastEnd_ = end;
    hasLastEnd_ = true;
}

bool UndoStack::undo(TextBuffer& buf) {
    if (undoOps_.empty()) return false;
    EditOp op = undoOps_.back();
    undoOps_.pop_back();

    Position insertedEnd = advance(op.start, op.inserted);
    buf.eraseRange(op.start, insertedEnd);
    buf.insertText(op.start, op.removed);
    buf.setCursor(advance(op.start, op.removed));

    redoOps_.push_back(op);
    hasLastEnd_ = false;
    return true;
}

bool UndoStack::redo(TextBuffer& buf) {
    if (redoOps_.empty()) return false;
    EditOp op = redoOps_.back();
    redoOps_.pop_back();

    Position removedEnd = advance(op.start, op.removed);
    buf.eraseRange(op.start, removedEnd);
    buf.insertText(op.start, op.inserted);
    buf.setCursor(advance(op.start, op.inserted));

    undoOps_.push_back(op);
    hasLastEnd_ = false;
    return true;
}

} // namespace editor
