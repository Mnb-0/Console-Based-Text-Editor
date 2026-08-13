#pragma once
#include <memory>
#include <string>
#include <vector>

namespace editor {

// An immutable, shareable copy of the buffer's lines, handed to worker
// threads so they never touch the live TextBuffer. Paired with a version
// number by the caller so late-arriving results can be detected as stale.
using BufferSnapshot = std::shared_ptr<const std::vector<std::string>>;

inline BufferSnapshot makeSnapshot(const std::vector<std::string>& lines) {
    return std::make_shared<const std::vector<std::string>>(lines);
}

} // namespace editor
