

#include <ncurses.h>
#include "Log.h"
#include "Editor.h"

Log* Log::s_instance = NULL;

void initNcurses();

int main(int argc, char** argv) {
    Log::instance();

    initNcurses();
    Editor * editor;

    if (argc > 1)
    {
        Log::instance()->logMessage("Provided filename\n");
        std::string filename = argv[1];
        editor = new Editor(filename);
    }
    else
    {
        editor = new Editor();
    }
    editor->printBuffer();
    getch();
    endwin();

    return 0;
}

void initNcurses() {
    initscr();
    noecho();
}