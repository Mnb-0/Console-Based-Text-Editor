#pragma once
#include <vector>

#include "spell/SpellChecker.h"

namespace editor {

class Document;

struct ViewState {
    int topLine = 0;
};

// Header rows reserved above the text area: key hints, status bar, and a
// suggestions/message line.
constexpr int kHeaderRows = 3;

// Scrolls `view` so the cursor stays within the visible viewport. Call
// before renderBuffer() whenever the cursor may have moved.
void ensureCursorVisible(const Document& doc, ViewState& view, int viewportRows);

// Draws only the visible slice of the buffer [topLine, topLine+viewportRows)
// and positions the terminal cursor from the document's actual cursor state
// - unlike the original's printTextContent(), which recomputed screen
// position by walking the whole buffer and lost track of the real cursor.
void renderBuffer(const Document& doc, ViewState& view, int viewportRows, int viewportCols,
                   const std::vector<MisspelledSpan>& misspellings);

} // namespace editor
