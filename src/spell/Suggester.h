#pragma once
#include <atomic>
#include <string>
#include <vector>

#include "spell/Dictionary.h"

namespace editor {

struct SuggestionResult {
    std::string word;
    bool correct = false;
    std::vector<std::string> suggestions;
};

// Substitution / omission / insertion / reversal suggestions - the same four
// strategies as the original spellCheck(), fixed at the source: an empty
// `word` returns immediately instead of underflowing `word.length() - 1`.
// Runs off the UI thread; `cancelled` is checked between passes so a stale
// request (the user moved on to another word) can bail out early.
SuggestionResult suggest(const Dictionary& dict, const std::string& word, const std::atomic<bool>& cancelled);

} // namespace editor
