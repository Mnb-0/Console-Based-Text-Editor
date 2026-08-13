#include "ui/StatusBar.h"

#include <ncurses.h>
#include <string>

#include "core/Document.h"
#include "ui/Screen.h"

namespace editor {

void drawStatusBar(int row, int cols, const Document& doc, bool dictReady, size_t dictWordCount,
                    const std::string& message) {
    (void)cols;
    move(row, 0);
    clrtoeol();

    Position cur = doc.buffer().cursor();
    std::string name = doc.hasFilename() ? doc.filename() : "[No Name]";
    std::string dictStatus = dictReady ? (std::to_string(dictWordCount) + " words") : "loading...";

    attron(COLOR_PAIR(PAIR_STATUS));
    printw("%s%s | Ln %d, Col %d | Dict: %s | %s",
           name.c_str(), doc.dirty() ? "*" : "",
           cur.row + 1, cur.col + 1,
           dictStatus.c_str(), message.c_str());
    attroff(COLOR_PAIR(PAIR_STATUS));
}

} // namespace editor
