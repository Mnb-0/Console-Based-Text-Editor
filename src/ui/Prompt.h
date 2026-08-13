#pragma once
#include <cstddef>
#include <string>

namespace editor {

// Bounded line input (replaces the original's unbounded getstr()). Returns
// the entered text, or an empty string if the user just pressed Enter with
// nothing typed - callers treat that as "cancelled".
std::string promptInput(int row, const std::string& label, size_t maxLen = 255);

// Blocks until the user answers y/n; ESC counts as "no".
bool promptYesNo(int row, const std::string& question);

} // namespace editor
