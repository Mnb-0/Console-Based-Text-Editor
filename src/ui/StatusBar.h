#pragma once
#include <cstddef>
#include <string>

namespace editor {

class Document;

void drawStatusBar(int row, int cols, const Document& doc, bool dictReady, size_t dictWordCount,
                    const std::string& message);

} // namespace editor
