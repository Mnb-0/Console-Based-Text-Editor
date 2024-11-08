#include <iostream>
#include <string>
#include <fstream>
#include <ncurses.h>
using namespace std;

int main() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    int ch;
    while ((ch = getch()) != 'q') {
        printw("Key: %c\n", ch);
    }
    endwin();
    return 0;
}