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
    int y = 0;
    int x = m_x;

    for (std::string line : m_buffer->m_lines)
    {
        move(y, 0);
        clrtoeol();
        if (y == m_y && m_mode == 'i')
        {
            printw(line.substr(0, m_x).c_str());
            if (line.length() > m_x )
            {
                addch(line.at(m_x) | A_REVERSE);
                Log::instance()->logMessage("m_y: %d Curr length: %d, end_length: %d\n", m_x,
                                            line.length(), line.length() - m_x - 1);

                printw(line.substr(m_x + 1, line.length() - m_x - 1).c_str());
            }
        }
        else
        {
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
    std::stringstream ss;
    char chrToInsert = (char) chr;
    ss << chrToInsert;
    // "What\n??is your name?"
    std::string curr_line = m_buffer->m_lines[m_y];
    if (chrToInsert == '\n')
    {
        std::string stringBeforeEnter = curr_line.substr(0, m_x );
//        stringBeforeEnter.append(ss.str());
        std::string stringAfterEnter = curr_line.substr(m_x);
        m_buffer->m_lines[m_y] = stringBeforeEnter;
        m_buffer->m_lines.insert(m_buffer->m_lines.begin() + m_y + 1, stringAfterEnter);
        m_y++;
        m_x = 0;
    }
    else
    {
        std::string string = m_buffer->m_lines[m_y] = curr_line.insert(m_x, ss.str());
        m_x++;
    }
    Log::instance()->logMessage("Current chr to insert: %s\n", ss.str().c_str());




}


void Editor::moveUp()
{
    int minY = 0;
    if (m_y > minY)
    {
        Log::instance()->logMessage(" Editor::moveUp: first if\n");
        // if the line above is shorter then current line
        int maxXlineAbove = m_buffer->m_lines[m_y - 1].length() - 1;
        if (m_x > maxXlineAbove)
        {
            Log::instance()->logMessage("Line above is shorter\n");
            m_x = maxXlineAbove;
        }
        m_y = m_y - 1;
        Log::instance()->logMessage("After moving up current m_y: %d\n", m_y);
    }
    else
    {
        m_y = 0;
    }
    Log::instance()->logMessage("Moving to: m_y=%d m_x=%d\n", m_y, m_x);
    move(m_y, m_x);
}

void Editor::moveDown()
{
    int maxBufferLines = m_buffer->m_lines.size();
    int maxScreenSize = LINES;
    int newY = m_y + 1;
    Log::instance()->logMessage("buffer_size: %d LINES: %d, new_y: %d\n", m_buffer->m_lines.size(), LINES, newY);
    if (newY >= maxBufferLines || newY >= maxScreenSize) // TODO remove '=' by removing -1 from above variables
    {
        m_y = std::min(maxBufferLines, maxScreenSize);
    }
    else
    {
        // if the line below is shorter then current line
        int maxXlineBelow = m_buffer->m_lines[m_y + 1].length() - 1;
        Log::instance()->logMessage("Length line below: %d\n",m_buffer->m_lines[m_y + 1].length()  );
        if (m_x > maxXlineBelow)
        {
            m_x = maxXlineBelow;
        }
        m_y = newY;
    }
    Log::instance()->logMessage("Going to move to m_y=%d m_x=%d\n", m_y, m_x);
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
    if (newX >= maxLineSize)
    {
        m_x = maxLineSize - 1;
        // TODO allow wrapping as a feature
    }
    else
    {
        m_x = m_x + 1;
    }
    move(m_y, m_x);
}