#pragma once
#include <string>
#include <unordered_set>

namespace editor {

// Word list backed by unordered_set for O(1) average lookup. All words are
// lowercased at load time so lookups are case-insensitive - "This" at the
// start of a sentence is not permanently flagged as misspelled.
class Dictionary {
public:
    void loadFromFile(const std::string& path);
    bool contains(const std::string& word) const;
    size_t size() const { return words_.size(); }

    static std::string normalize(const std::string& word);

private:
    std::unordered_set<std::string> words_;
};

} // namespace editor
