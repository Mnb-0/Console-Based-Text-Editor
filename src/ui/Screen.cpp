#include "ui/Screen.h"

#include <ncurses.h>

namespace editor {

Screen::Screen() {
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);
    set_escdelay(25);
    initColors();
}

Screen::~Screen() {
    endwin();
}

void Screen::initColors() {
    if (has_colors()) {
        start_color();
        use_default_colors();
        init_pair(PAIR_MISSPELLED, COLOR_RED, -1);
        init_pair(PAIR_STATUS, COLOR_BLACK, COLOR_CYAN);
        init_pair(PAIR_SELECTION, COLOR_BLACK, COLOR_WHITE);
    }
}

} // namespace editor
