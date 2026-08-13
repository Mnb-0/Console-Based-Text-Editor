#pragma once
#include <string>
#include <vector>

namespace editor {

struct LoadResult {
    bool success;
    std::vector<std::string> lines;
    std::string error;
    bool trailingNewline = true; // whether the source file's last byte was '\n'
};

struct SaveResult {
    bool success;
    std::string error;
};

LoadResult loadFile(const std::string& path);
SaveResult saveFile(const std::string& path, const std::vector<std::string>& lines, bool trailingNewline = true);

} // namespace editor
