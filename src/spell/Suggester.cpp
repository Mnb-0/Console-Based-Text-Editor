#include "spell/Suggester.h"

#include <algorithm>
#include <cctype>

namespace editor {

SuggestionResult suggest(const Dictionary& dict, const std::string& word, const std::atomic<bool>& cancelled) {
    SuggestionResult result;
    result.word = word;
    if (word.empty()) return result;

    if (dict.contains(word)) {
        result.correct = true;
        return result;
    }

    // Substitution: swap each letter for every other letter.
    for (size_t i = 0; i < word.size() && !cancelled; ++i) {
        for (char c = 'a'; c <= 'z'; ++c) {
            if (c == std::tolower(static_cast<unsigned char>(word[i]))) continue;
            std::string mod = word;
            mod[i] = c;
            if (dict.contains(mod)) result.suggestions.push_back(mod);
        }
    }

    // Omission: drop each letter.
    for (size_t i = 0; i < word.size() && !cancelled; ++i) {
        std::string mod = word.substr(0, i) + word.substr(i + 1);
        if (dict.contains(mod)) result.suggestions.push_back(mod);
    }

    // Insertion: add a letter at each position.
    for (size_t i = 0; i <= word.size() && !cancelled; ++i) {
        for (char c = 'a'; c <= 'z'; ++c) {
            std::string mod = word.substr(0, i) + c + word.substr(i);
            if (dict.contains(mod)) result.suggestions.push_back(mod);
        }
    }

    // Reversal: swap each pair of adjacent letters.
    if (word.size() > 1) {
        for (size_t i = 0; i + 1 < word.size() && !cancelled; ++i) {
            std::string mod = word;
            std::swap(mod[i], mod[i + 1]);
            if (dict.contains(mod)) result.suggestions.push_back(mod);
        }
    }

    std::sort(result.suggestions.begin(), result.suggestions.end());
    result.suggestions.erase(std::unique(result.suggestions.begin(), result.suggestions.end()),
                              result.suggestions.end());
    if (result.suggestions.size() > 8) result.suggestions.resize(8);

    return result;
}

} // namespace editor
