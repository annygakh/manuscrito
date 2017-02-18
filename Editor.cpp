//
// Created by Anaid Gakhokidze on 2016-11-16.
//

#include "Editor.h"
#include "Log.h"
#include <ncurses.h>
#include <algorithm>
#include <sstream>

Editor::Editor()
    : m_mode('n')
    , m_x(0)
    , m_y(0)
{
    m_buffer = new Buffer();
}

Editor::Editor(std::string filename)
    : m_mode('n')
    , m_x(0)
    , m_y(0)
{
    m_buffer = new Buffer(filename);
}

void Editor::printBuffer()
{
    int x = 0, y = 0;

    for (std::string line : m_buffer->m_lines)
    {
        move(y, x);
        if (y == m_y && m_mode == 'i') {
            printw(line.substr(0, m_x).c_str());
            if (line.length() > m_x + 1) {
                addch(line.at(m_x) | A_BLINK);
                Log::instance()->logMessage("m_y: %d Curr length: %d, end_length: %d\n", m_x, line.length(), line.length() - m_x - 1);

                printw(line.substr(m_x + 1, line.length() - m_x - 1).c_str());
            }
            Log::instance()->logMessage("here\n");
        } else {
            Log::instance()->logMessage("and here\n");

            printw(line.c_str());
        }
        y++;
    }
    move(m_y, m_x);
    refresh();
}

void Editor::handleInput(int chr)
{
    switch (chr)
    {
        case KEY_LEFT:
            Log::instance()->logMessage("Moving left\n");
            moveLeft();
            return;
        case KEY_RIGHT:
            Log::instance()->logMessage("Moving right\n");
            moveRight();
            return;
        case KEY_DOWN:
            Log::instance()->logMessage("Moving dowssssn\n");
            moveDown();
            return;
        case KEY_UP:
            Log::instance()->logMessage("Moving up\n");
            moveUp();
            return;
        default:
            break;
    }

    Log::instance()->logMessage("Chr %c\n", chr);

    switch (m_mode)
    {
        case 'n':
            handleInputInNormalMode(chr);
            break;
        case 'i':
            handleInputInInsertMode(chr);
            break;
        default:
            break;
    }
}

void Editor::handleInputInNormalMode(int chr)
{
    switch (chr)
    {
        case 'x':
            m_mode = 'x';
            break;
        case 'i':
            m_mode = 'i';
            Log::instance()->logMessage("Insert mode \n");
            break;
        default:
            // TODO add more
            break;
    }

}

void Editor::handleInputInInsertMode(int chr)
{
    Log::instance()->logMessage("Curr y: %d curr_x: %d\n", m_y, m_x);


    std::stringstream ss;
    ss << (char) chr;
    std::string curr_line = m_buffer->m_lines[m_y];

    m_buffer->m_lines[m_y] = curr_line.insert(m_x, ss.str());
    m_x++;


}


void Editor::moveUp()
{
    int minY = 0;
    if (m_y > minY)
    {
        // if the line above is shorter then current line
        int maxXlineAbove = m_buffer->m_lines[m_y - 1].length() - 1;
        if (m_x > maxXlineAbove)
        {
            m_x = maxXlineAbove;
        }
        m_y = m_y - 1;
    }
    else
    {
        m_y = 0;
    }
    move(m_y, m_x);
}

void Editor::moveDown()
{
    int maxBufferLines = m_buffer->m_lines.size() - 1;
    int maxScreenSize = LINES - 1;
    int newY = m_y + 1;
    if (newY >= maxBufferLines || newY >= maxScreenSize) // TODO remove '=' by removing -1 from above variables
    {
        m_y = std::min(maxBufferLines, maxScreenSize);
    }
    else
    {
        // if the line below is shorter then current line
        int maxXlineBelow = m_buffer->m_lines[m_y + 1].length() - 1;
        if (m_x > maxXlineBelow)
        {
            m_x = maxXlineBelow;
        }
        m_y = newY;
    }
    move(m_y, m_x);
}

void Editor::moveLeft()
{
    int newX = m_x - 1;
    if (newX < 0)
    {
        m_x = 0;
        // TODO allow wrapping
    }
    else
    {
        m_x = m_x - 1;
    }
    move(m_y, m_x);
}

void Editor::moveRight()
{
    int newX = m_x + 1;
    int maxLineSize = m_buffer->m_lines[m_y].length();
    if (newX > maxLineSize)
    {
        m_x = maxLineSize;
        // TODO allow wrapping as a feature
    }
    else
    {
        m_x = m_x + 1;
    }
    move(m_y, m_x);
}