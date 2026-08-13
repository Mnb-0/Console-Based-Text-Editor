#include "ui/Renderer.h"

#include <algorithm>
#include <ncurses.h>

#include "core/Document.h"
#include "ui/Screen.h"

namespace editor {

void ensureCursorVisible(const Document& doc, ViewState& view, int viewportRows) {
    Position cur = doc.buffer().cursor();
    if (cur.row < view.topLine) view.topLine = cur.row;
    if (cur.row >= view.topLine + viewportRows) view.topLine = cur.row - viewportRows + 1;
    if (view.topLine < 0) view.topLine = 0;
}

void renderBuffer(const Document& doc, ViewState& view, int viewportRows, int viewportCols,
                   const std::vector<MisspelledSpan>& misspellings) {
    const auto& lines = doc.buffer().lines();
    Position cur = doc.buffer().cursor();

    bool hasSel = doc.hasSelection();
    Position selA{}, selB{};
    if (hasSel) {
        auto range = doc.selectionRange();
        selA = range.first;
        selB = range.second;
    }

    for (int screenRow = 0; screenRow < viewportRows; ++screenRow) {
        int docRow = view.topLine + screenRow;
        move(kHeaderRows + screenRow, 0);
        clrtoeol();
        if (docRow >= static_cast<int>(lines.size())) continue;

        const std::string& line = lines[docRow];
        std::vector<bool> bad(line.size(), false);
        for (const auto& span : misspellings) {
            if (span.row != docRow) continue;
            for (int c = span.colStart; c < span.colEnd && c < static_cast<int>(bad.size()); ++c) bad[c] = true;
        }

        int maxCol = std::min(static_cast<int>(line.size()), viewportCols);
        for (int col = 0; col < maxCol; ++col) {
            bool selected = hasSel &&
                (docRow > selA.row || (docRow == selA.row && col >= selA.col)) &&
                (docRow < selB.row || (docRow == selB.row && col < selB.col));

            int attrs = A_NORMAL;
            if (selected) attrs = COLOR_PAIR(PAIR_SELECTION) | A_REVERSE;
            else if (bad[col]) attrs = COLOR_PAIR(PAIR_MISSPELLED) | A_UNDERLINE;

            attron(attrs);
            mvaddch(kHeaderRows + screenRow, col, line[col]);
            attroff(attrs);
        }
    }

    int screenCursorRow = kHeaderRows + (cur.row - view.topLine);
    int screenCursorCol = std::min(cur.col, viewportCols > 0 ? viewportCols - 1 : 0);
    move(screenCursorRow, screenCursorCol);
}

} // namespace editor
