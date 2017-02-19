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
    , m_commandSoFar("")
    , m_pendingAction(NONE)
{
    m_buffer = new Buffer();
}

Editor::Editor(std::string filename)
        : m_mode('n')
        , m_x(0)
        , m_y(0)
        , m_commandSoFar("")
        , m_pendingAction(NONE)
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
        case 'p':
            status = handlePrompt();
            break;
        default:
            status = "Unknown status";
            break;
    }

    attrset(A_UNDERLINE | A_STANDOUT);
    int y_status = LINES - 1;
    int x_status = 0;
    move(y_status, x_status);
    clrtoeol();
    mvprintw(y_status, x_status, status.c_str());
    attroff(A_UNDERLINE | A_STANDOUT);
    move(m_x, m_y);
}

std::string Editor::handlePrompt()
{
    Log::instance()->logMessage("Handle prompt\n");
    std::string status;
    switch (m_pendingAction)
    {
        case SAVE_FILE:
            status = "Enter name for file:";
            status.append(m_commandSoFar);
            break;
        case NONE:
        default:
            status = "Unknown status";
            break;
    }
    m_y = LINES - 1;
    m_x = status.length();

    return status;
}

void Editor::handleInput(int chr)
{
    std::string keyName = keyname(chr);
    Log::instance()->logMessage("You pressed:%s\n", keyName.c_str());

    if (m_mode == 'p' && (char) chr != '\n')
    {
        std::stringstream ss;
        ss << (char) chr;
        m_commandSoFar.append(ss.str());
    }
    else if (m_mode == 'p' && chr == '\n')
    {
        m_mode = m_previousMode;

        // TODO handle end of command in prompt mode
        switch(m_pendingAction)
        {
            case SAVE_FILE:
                saveFile(m_commandSoFar);
                m_x = m_prevX;
                m_y = m_prevY;
                break;
            default:
                break;
        }
        m_commandSoFar.clear();
        return;
    }

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
            if (keyName == "^R")
            {
                Log::instance()->logMessage("Saving the file\n");
                m_previousMode = m_mode;
                m_pendingAction = SAVE_FILE;
                m_mode = 'p';
                m_prevX = m_x;
                m_prevY = m_y;
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

void Editor::saveFile(std::string filename)
{
    m_buffer->saveFile(filename);
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
        // Allow wrapping - moving left at the beginning of the line
        // leads us to end up at the end of the line above us.
        if (m_y > 0)
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
    if (newX >= maxLineSize) // Moving right at the end of the line
    {
        // Allow wrapping - moving right at the end of one line
        // leads to ending up at the beginning of the next line
        if (m_y + 1 < m_buffer->m_lines.size())
        {
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