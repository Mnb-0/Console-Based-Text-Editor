#include <atomic>
#include <cstdio>
#include <fstream>

#include "harness.h"
#include "spell/Dictionary.h"
#include "spell/Suggester.h"

using namespace editor;

TEST(suggest_flags_correct_word) {
    const char* path = "test_suggest_tmp1.txt";
    {
        std::ofstream f(path);
        f << "cat\ndog\n";
    }
    Dictionary dict;
    dict.loadFromFile(path);
    std::atomic<bool> cancelled{false};
    auto r = suggest(dict, "cat", cancelled);
    CHECK(r.correct);
    std::remove(path);
}

TEST(suggest_empty_word_does_not_crash) {
    // Regression test for the original bug: spellCheck's reversal loop used
    // `word.length() - 1` on an unsigned length, which underflowed to a huge
    // value for an empty word and walked far past the string's bounds.
    Dictionary dict;
    std::atomic<bool> cancelled{false};
    auto r = suggest(dict, "", cancelled);
    CHECK(!r.correct);
    CHECK(r.suggestions.empty());
}

TEST(suggest_offers_substitution) {
    const char* path = "test_suggest_tmp2.txt";
    {
        std::ofstream f(path);
        f << "cat\n";
    }
    Dictionary dict;
    dict.loadFromFile(path);
    std::atomic<bool> cancelled{false};
    auto r = suggest(dict, "cot", cancelled);
    CHECK(!r.correct);
    bool found = false;
    for (auto& s : r.suggestions) {
        if (s == "cat") found = true;
    }
    CHECK(found);
    std::remove(path);
}

TEST(suggest_respects_cancellation) {
    Dictionary dict;
    std::atomic<bool> cancelled{true};
    auto r = suggest(dict, "somewhatlongword", cancelled);
    CHECK(r.suggestions.empty());
}

TEST(suggest_single_letter_word_does_not_crash) {
    Dictionary dict;
    std::atomic<bool> cancelled{false};
    auto r = suggest(dict, "a", cancelled); // reversal loop must not run on a 1-char word
    CHECK(!r.correct);
}
