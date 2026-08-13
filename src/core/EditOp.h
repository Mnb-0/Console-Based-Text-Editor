#pragma once
#include <string>

#include "core/TextBuffer.h"

namespace editor {

// A single reversible edit: `removed` was replaced by `inserted` at `start`.
// Plain inserts have an empty `removed`; plain deletes have an empty
// `inserted`. This one shape covers typing, backspace/delete, paste, and
// find-and-replace.
struct EditOp {
    Position start;
    std::string removed;
    std::string inserted;
};

} // namespace editor
