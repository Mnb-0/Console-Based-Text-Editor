#include "spell/Dictionary.h"

#include <algorithm>
#include <cctype>
#include <fstream>

namespace editor {

std::string Dictionary::normalize(const std::string& word) {
    std::string out = word;
    std::transform(out.begin(), out.end(), out.begin(), [](unsigned char c) { return std::tolower(c); });
    return out;
}

void Dictionary::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return;

    std::string word;
    while (file >> word) {
        words_.insert(normalize(word));
    }
}

bool Dictionary::contains(const std::string& word) const {
    return words_.count(normalize(word)) > 0;
}

} // namespace editor
