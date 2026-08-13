#include "ui/Prompt.h"

#include <ncurses.h>
#include <vector>

namespace editor {

// The main loop runs getch() with a 16ms timeout so it can poll worker
// events between keystrokes. A modal prompt has nothing else to do while
// waiting, so it switches to blocking reads for its duration and restores
// the poll timeout on the way out.

std::string promptInput(int row, const std::string& label, size_t maxLen) {
    nodelay(stdscr, FALSE);
    echo();
    move(row, 0);
    clrtoeol();
    printw("%s", label.c_str());
    refresh();

    std::vector<char> buf(maxLen + 1, '\0');
    getnstr(buf.data(), static_cast<int>(maxLen));

    noecho();
    timeout(16);
    return std::string(buf.data());
}

bool promptYesNo(int row, const std::string& question) {
    nodelay(stdscr, FALSE);
    move(row, 0);
    clrtoeol();
    printw("%s (y/n) ", question.c_str());
    refresh();

    int ch;
    do {
        ch = getch();
    } while (ch != 'y' && ch != 'Y' && ch != 'n' && ch != 'N' && ch != 27);

    timeout(16);
    return ch == 'y' || ch == 'Y';
}

} // namespace editor
