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
    , m_enteredPromptMode(false)
    , m_posWithinCommand(0)
{
    m_buffer = new Buffer();
}

Editor::Editor(std::string filename)
        : m_mode('n')
        , m_x(0)
        , m_y(0)
        , m_commandSoFar("")
        , m_pendingAction(NONE)
        , m_enteredPromptMode(false)
        , m_posWithinCommand(0)
{
    m_buffer = new Buffer(filename);
}

void Editor::printBuffer()
{
    int y = 0;

    for (std::string line : m_buffer->m_lines)
    {
        mvprintw(y, 0, line.c_str());
        y++;
    }
    move(m_y, m_x);
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
    mvprintw(y_status, x_status, status.c_str());
    attroff(A_UNDERLINE | A_STANDOUT);
    move(m_y, m_x);
}

std::string Editor::handlePrompt()
{
    Log::instance()->logMessage("Handle prompt\n");
    std::string status;
    switch (m_pendingAction)
    {
        case SAVE_FILE:
            status = "Enter name for file:";
            m_prompt = "Enter name for file:";
            status.append(m_commandSoFar);
            break;
        case NONE:
        default:
            status = "Unknown status";
            break;
    }
    if (!m_enteredPromptMode)
    {
        m_y = LINES - 1;
        m_x = status.length();
    }

    return status;
}

void Editor::handleInput(int chr)
{
    std::string keyName = keyname(chr);
    Log::instance()->logMessage("You pressed: int=%d keyname=%s\n", chr, keyName.c_str());

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
            Log::instance()->logMessage("Moving down\n");
            moveDown();
            return;
        case KEY_UP:
            Log::instance()->logMessage("Moving up\n");
            moveUp();
            return;
        case '\n':
            handleEnterKey();
            return;
        case 8:
        case 127:
            handleDeleteKey();
            return;
        case 27:
            handleEscapeKey();
            return;
        default:
            if (keyName == "^R" && m_mode != 'p')
            {
                Log::instance()->logMessage("Saving the file\n");
                m_previousMode = m_mode;
                m_pendingAction = SAVE_FILE;
                m_mode = 'p';
                m_prevX = m_x;
                m_prevY = m_y;
                return;
            }
            else if (keyName == "^R" && m_mode == 'p')
            {
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
        case 'p':
            handleInputInPromptMode(chr);
            break;
        default:
            break;
    }
}

void Editor::handleEscapeKey()
{
    switch (m_mode)
    {
        case 'i':
            m_mode = 'n';
            break;
        case 'n':
            break;
        case 'p':
            // TODO refactor this, because it contains similar code to handleEnterKeyPromptMode()
            m_mode = m_previousMode;
            m_commandSoFar.clear();
            m_prompt.clear();
            m_posWithinCommand = 0;
            m_enteredPromptMode = false;
            m_x = m_prevX;
            m_y = m_prevY;
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
        case 'p':
            m_mode = 'p';
            break;
        default:
            break;
    }
}

void Editor::saveFile(std::string filename)
{
    m_buffer->setFilename(filename);
    m_buffer->saveFile();
}

void Editor::handleInputInInsertMode(int chr)
{
    std::stringstream ss;
    char chrToInsert = (char) chr;
    ss << chrToInsert;
    std::string curr_line = m_buffer->m_lines[m_y];
    m_buffer->m_lines[m_y] = curr_line.insert(m_x, ss.str());
    m_x++;
}

void Editor::handleInputInPromptMode(int chr)
{
    m_enteredPromptMode = true;
    std::stringstream ss;
    ss << (char) chr;
    m_commandSoFar.insert(m_posWithinCommand, ss.str());
    m_posWithinCommand++;
    m_x++;
}


void Editor::handleMoveUpNormalInsert()
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
}

void Editor::moveUp()
{
    switch (m_mode)
    {
        case 'i':
        case 'n':
            handleMoveUpNormalInsert();
            break;
        case 'p':
            // TODO allow scrolling through history
            break;
        default:
            break;
    }
}

void Editor::handleEnterKey()
{
    switch (m_mode)
    {
        case 'i':
            handleEnterKeyInsertMode();
            break;
        case 'n':
            break;
        case 'p':
            handleEnterKeyPromptMode();
            break;
        default:
            break;
    }
}

void Editor::handleEnterKeyInsertMode()
{
    std::string curr_line = m_buffer->m_lines[m_y];
    std::string stringBeforeEnter = curr_line.substr(0, m_x );
//        stringBeforeEnter.append(ss.str());
    std::string stringAfterEnter = curr_line.substr(m_x);
    m_buffer->m_lines[m_y] = stringBeforeEnter;
    m_buffer->m_lines.insert(m_buffer->m_lines.begin() + m_y + 1, stringAfterEnter);
    m_y++;
    m_x = 0;

}

void Editor::handleEnterKeyPromptMode()
{
    m_mode = m_previousMode;

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
    m_prompt.clear();
    m_posWithinCommand = 0;
    m_enteredPromptMode = false;
}

void Editor::handleDeleteKeyInsertMode()
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

void Editor::handleDeleteKeyPromptMode()
{
    std::string promptAndCommand = m_prompt;
    promptAndCommand.append(m_commandSoFar);
    int status_line_length = promptAndCommand.length();

    int prompt_length = m_prompt.length();
    if (m_x > prompt_length && m_x <= status_line_length)
    {
        m_commandSoFar.erase(m_posWithinCommand - 1, 1);
        m_x--;
        m_posWithinCommand--;
    }
}

void Editor::handleDeleteKey()
{
    switch (m_mode)
    {
        case 'i':
            handleDeleteKeyInsertMode();
            break;
        case 'n':
            break;
        case 'p':
            handleDeleteKeyPromptMode();
            break;
        default:
            break;
    }
}

void Editor::handleMoveDownNormalInsert()
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

void Editor::moveDown()
{
    switch (m_mode)
    {
        case 'i':
        case 'n':
            handleMoveDownNormalInsert();
            break;
        case 'p':
            // TODO allow scrolling through history
            break;
        default:
            break;
    }
}

void Editor::handleMoveLeftNormalInsert()
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

void Editor::moveLeft()
{
    switch (m_mode)
    {
        case 'i':
        case 'n':
            handleMoveLeftNormalInsert();
            break;
        case 'p':
            handleMoveLeftPromptMode();
            break;
        default:
            break;
    }
}

void Editor::handleMoveRightNormalInsert()
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

void Editor::handleMoveRightPromptMode()
{
    std::string promptAndCommand = m_prompt;
    promptAndCommand.append(m_commandSoFar);
    int status_line_length = promptAndCommand.length();
    bool cursor_at_the_end_of_status_line = m_x  == status_line_length ;
    if (!cursor_at_the_end_of_status_line)
    {
        m_x++;
        m_posWithinCommand++;
    }
}

void Editor::handleMoveLeftPromptMode()
{
    int status_length = m_prompt.length();
    if (m_x > status_length)
    {
        m_x--;
        m_posWithinCommand--;
    }
}

void Editor::moveRight()
{
    switch (m_mode)
    {
        case 'i':
        case 'n':
            handleMoveRightNormalInsert();
            break;
        case 'p':
            handleMoveRightPromptMode();
            break;
        default:
            break;
    }
}