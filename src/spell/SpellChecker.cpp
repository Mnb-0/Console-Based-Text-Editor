#include "spell/SpellChecker.h"

#include <cctype>

namespace editor {

namespace {
bool isWordChar(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) || c == '\'';
}
} // namespace

std::vector<MisspelledSpan> scanBuffer(const std::vector<std::string>& lines, const Dictionary& dict,
                                        const std::atomic<bool>& cancelled) {
    std::vector<MisspelledSpan> spans;

    for (int r = 0; r < static_cast<int>(lines.size()); ++r) {
        if (cancelled) return {};
        const std::string& line = lines[r];
        int i = 0;
        int n = static_cast<int>(line.size());
        while (i < n) {
            if (!isWordChar(line[i])) {
                i++;
                continue;
            }
            int start = i;
            while (i < n && isWordChar(line[i])) i++;
            std::string word = line.substr(start, i - start);
            if (!dict.contains(word)) spans.push_back({r, start, i});
        }
    }
    return spans;
}

} // namespace editor
