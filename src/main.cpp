#include <string>

#include "ui/Editor.h"

int main(int argc, char** argv) {
    std::string file = argc > 1 ? argv[1] : "";
    editor::Editor editor(file);
    editor.run();
    return 0;
}
