#include "io/FileIO.h"

#include <fstream>
#include <iterator>

namespace editor {

LoadResult loadFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return {false, {}, "Failed to open '" + path + "' for reading."};
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    if (file.bad()) {
        return {false, {}, "Error reading '" + path + "'."};
    }

    LoadResult result;
    result.success = true;
    result.trailingNewline = !content.empty() && content.back() == '\n';

    // Split on '\n' without emitting a spurious trailing empty line when the
    // file ends with a newline (the usual case) - getline-based splitting
    // can't distinguish "ends with \n" from "doesn't", which used to make a
    // save silently drop the file's trailing newline.
    size_t start = 0;
    for (size_t i = 0; i < content.size(); ++i) {
        if (content[i] == '\n') {
            result.lines.push_back(content.substr(start, i - start));
            start = i + 1;
        }
    }
    if (start < content.size()) {
        result.lines.push_back(content.substr(start));
    } else if (content.empty()) {
        result.lines.push_back("");
    }

    return result;
}

SaveResult saveFile(const std::string& path, const std::vector<std::string>& lines, bool trailingNewline) {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return {false, "Failed to open '" + path + "' for writing."};
    }

    for (size_t i = 0; i < lines.size(); ++i) {
        file << lines[i];
        if (i + 1 < lines.size() || trailingNewline) file << '\n';
    }

    if (file.fail()) {
        return {false, "Error writing '" + path + "'."};
    }
    return {true, ""};
}

} // namespace editor
