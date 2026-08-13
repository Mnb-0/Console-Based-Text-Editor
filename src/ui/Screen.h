#pragma once

namespace editor {

enum ColorPairId { PAIR_MISSPELLED = 1, PAIR_STATUS = 2, PAIR_SELECTION = 3 };

// RAII wrapper around initscr()/endwin() so the terminal is always restored,
// including on exceptions. Uses raw() rather than cbreak() so Ctrl+C/Ctrl+Z
// reach the editor as plain key codes instead of raising SIGINT/SIGTSTP -
// several of this editor's bindings (undo, etc.) live on control keys that
// would otherwise be intercepted by the terminal driver.
class Screen {
public:
    Screen();
    ~Screen();

    Screen(const Screen&) = delete;
    Screen& operator=(const Screen&) = delete;

private:
    void initColors();
};

} // namespace editor
