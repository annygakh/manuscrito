//
// Created by Anaid Gakhokidze on 2016-11-16.
//

#include "Editor.h"
#include "Log.h"
#include <ncurses.h>
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

void Editor::updateStatus()
{
    Log::instance()->logMessage("updateStatus\n");
    std::string status;
    switch (m_mode)
    {
        case 'n':
            status = "Normal mode";
            break;
        case 'i':
            status = "Insert mode";
            break;
        case 'x':
            status = "Exiting";
            break;
        default:
            status = "Unknown status";
            break;
    }

    attrset(A_UNDERLINE | A_STANDOUT);
    mvprintw(LINES - 1, 0, status.c_str());
    attroff(A_UNDERLINE | A_STANDOUT);
}

void Editor::handleInput(int chr)
{
    std::string name = keyname(chr);
    Log::instance()->logMessage("You pressed:%s\n", name.c_str());

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
            if (name == "^R")
            {
                Log::instance()->logMessage("Saving the file\n");
                saveFile();
                return;
            }
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

void Editor::saveFile()
{
    m_buffer->saveFile();
}

void Editor::handleInputInInsertMode(int chr)
{
    std::stringstream ss;
    char chrToInsert = (char) chr;
    ss << chrToInsert;
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
    else if (chr == 127 || chr == 8)
    {
        handleDeleteKey();
    }
    else
    {
        m_buffer->m_lines[m_y] = curr_line.insert(m_x, ss.str());
        m_x++;
    }

}


void Editor::moveUp()
{
    if (m_y > 0)
    {

        int maxXlineAbove = m_buffer->m_lines[m_y - 1].length() - 1;
        if (maxXlineAbove == -1) // If the line above is empty
        {
            m_x = 0;
        }
        else if (m_x > maxXlineAbove) // if the line above is shorter then current line
        {

            m_x = maxXlineAbove;
        }
        m_y = m_y - 1;
    }
    else // If we are at the top of our file
    {
        m_y = 0;
    }
    move(m_y, m_x);
}

void Editor::handleDeleteKey()
{
    if (m_x == 0)
    {
        if (m_y == 0) // If we are at 0,0 and want to delete
        {
            return;
        }
        else // Need to join current line with previous line
        {
            std::string lineAbove = m_buffer->m_lines[m_y - 1];
            m_x = lineAbove.length();
            std::string currLine = m_buffer->m_lines[m_y];
            m_buffer->m_lines[m_y - 1] = lineAbove.append(currLine);
            m_buffer->m_lines.erase(m_buffer->m_lines.begin() + m_y);
            m_y--;
        }
    }
    else // Deleting just one character
    {
        std::string currLine = m_buffer->m_lines[m_y];
        m_buffer->m_lines[m_y] = currLine.erase(m_x - 1, 1);
        m_x--;

    }
    move(m_y, m_x);
}

void Editor::moveDown()
{
    int maxBufferLines = m_buffer->m_lines.size();
    int maxScreenSize = LINES;
    int newY = m_y + 1;
    Log::instance()->logMessage("buffer_size: %d LINES: %d, new_y: %d\n", m_buffer->m_lines.size(), LINES, newY);
    // TODO remove '=' by removing -1 from above variables
    bool outsideBounds = newY >= maxBufferLines || newY >= maxScreenSize;
    if (outsideBounds)
    {
        return;
    }
    else
    {
        // if the line below is shorter then current line
        int maxXlineBelow = m_buffer->m_lines[m_y + 1].length() - 1;
        if (maxXlineBelow == -1)
        {
            m_x = 0;
        }
        else if (m_x > maxXlineBelow)
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
    if (newX < 0) // If we are at the beginning of the line
    {
        if (m_y > 0) // If we are not at the first line in the file
        {
            int lineSizeLineAbove = m_buffer->m_lines[m_y-1].length();
            if (lineSizeLineAbove == 0)
            {
                m_x = 0;
            }
            else
            {
                m_x = lineSizeLineAbove + 1;
            }
            m_y--;
        }
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
    unsigned long maxLineSize = m_buffer->m_lines[m_y].length();
    if (newX >= maxLineSize)
    {
        if (m_y + 1 < m_buffer->m_lines.size()) {
            m_x = 0;
            m_y++;
        }
    }
    else
    {
        m_x = m_x + 1;
    }
    move(m_y, m_x);
}