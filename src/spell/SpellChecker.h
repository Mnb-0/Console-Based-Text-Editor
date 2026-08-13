#pragma once
#include <atomic>
#include <string>
#include <vector>

#include "spell/Dictionary.h"

namespace editor {

struct MisspelledSpan {
    int row;
    int colStart;
    int colEnd;
};

// Tokenizes every line and flags words absent from the dictionary. Intended
// to run on a worker thread against an immutable snapshot of the buffer.
std::vector<MisspelledSpan> scanBuffer(const std::vector<std::string>& lines, const Dictionary& dict,
                                        const std::atomic<bool>& cancelled);

} // namespace editor
