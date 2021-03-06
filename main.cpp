

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
        editor = new Editor(filename, LINES, COLS);
    }
    else
    {
        editor = new Editor(LINES, COLS);
    }
    editor->printBuffer();
    editor->updateStatus();
    refresh();

    while (editor->getMode() != 'x')
    {
        editor->setMaxLines(LINES);
        editor->setMaxCols(COLS);
        Log::instance()->logMessage("New LINES: %d, COLS: %d\n", LINES, COLS);
        int chr = getch();
        editor->handleInput(chr);
        clear(); // TODO figure out an efficient way to only clear out what changed on the screen
        editor->printBuffer();
        editor->updateStatus();
        refresh();
    }
    refresh();
    endwin();

    return 0;
}

void initNcurses() {
    initscr();
    noecho();

    keypad(stdscr, true);
}