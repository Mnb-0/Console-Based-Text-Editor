#include <cstdio>
#include <fstream>

#include "harness.h"
#include "spell/Dictionary.h"

using namespace editor;

TEST(normalize_lowercases) {
    CHECK_EQ(Dictionary::normalize("Hello"), std::string("hello"));
}

TEST(dictionary_loads_and_normalizes_case) {
    const char* path = "test_dictionary_tmp.txt";
    {
        std::ofstream f(path);
        f << "Hello\nWorld\nfoo\n";
    }
    Dictionary dict;
    dict.loadFromFile(path);
    CHECK(dict.contains("hello"));
    CHECK(dict.contains("HELLO"));
    CHECK(dict.contains("World"));
    CHECK(!dict.contains("missing"));
    std::remove(path);
}

TEST(dictionary_missing_file_is_harmless) {
    Dictionary dict;
    dict.loadFromFile("does_not_exist_hopefully.txt");
    CHECK_EQ(dict.size(), static_cast<size_t>(0));
}
