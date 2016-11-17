//
// Created by Anaid Gakhokidze on 2016-11-16.
//

#include "Editor.h"
#include "Log.h"
#include <ncurses.h>

Editor::Editor()
{
    m_buffer = new Buffer();
}

Editor::Editor(std::string filename)
{
    m_buffer = new Buffer(filename);
}

void Editor::printBuffer()
{
    int x = 0, y = 0;

    for (std::string line : m_buffer->m_lines)
    {
        move(y, x);
        printw(line.c_str());
        y++;
    }
    refresh();
}