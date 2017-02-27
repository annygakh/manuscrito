//
// Created by Anaid Gakhokidze on 2016-11-16.
//

#ifndef MANUSCRITO_EDITOR_H
#define MANUSCRITO_EDITOR_H

#include <string>
#include "Buffer.h"

enum Action
{
    NONE,
    SAVE_FILE
};

/*
 * Number of lines in the window that are allocated to editor specific features that are listed here.
 * The actual value is the sum of all lines that features require.
 * The format is as follows
 * # of lines - feature
 * 1 - status line
 *
 */
static int s_numLinesOccupied = 1;
static int s_numColsOccupied = 0;

class Editor {
private:
    Buffer * m_buffer;

    /*
     * x - exit mode
     * n - normal mode
     * i - insert mode
     * p - prompt mode
     * */
    char m_mode;
    char m_previousMode;

    // represents coordinates on the screen
    int m_x, m_y;
    int m_prevX, m_prevY;

    // represents current x and y within our buffer
    int m_bufferX, m_bufferY;
    int m_bufferTopY;
    int m_maxLines;
    int m_maxCols;


    // When the user writes anything while being prompted, it appears here
    std::string m_commandSoFar;
    int m_posWithinCommand;

    // Contains the prompt that user sees
    std::string m_prompt;
    bool m_enteredPromptMode;

    Action m_pendingAction;


    void handleInputInNormalMode(int);
    void handleInputInInsertMode(int);
    void handleInputInPromptMode(int);
    std::string handlePrompt();

    void handleDeleteKey();
    void handleEnterKey();
    void handleEscapeKey();

    void handleMoveUpNormalInsert();
    void handleMoveDownNormalInsert();
    void handleMoveLeftNormalInsert();
    void handleDeleteKeyInsertMode();
    void handleDeleteKeyPromptMode();
    void handleEnterKeyInsertMode();
    void handleEnterKeyPromptMode();

    void handleMoveRightNormalInsert();
    void handleMoveRightPromptMode();
    void handleMoveLeftPromptMode();

    void moveDown();
    void moveUp();
    void moveLeft();
    void moveRight();

public:
    Editor(int maxLines, int maxCols);
    Editor(std::string, int maxLines, int maxCols);

    char getMode() {
        return m_mode;
    }

    void setMaxLines(int maxLines) { m_maxLines = maxLines - s_numLinesOccupied; }
    void setMaxCols(int maxCols) { m_maxCols = maxCols - s_numColsOccupied; }

    void printBuffer();

    void updateStatus();

    void handleInput(int);

    void saveFile(std::string filename);

};


#endif //MANUSCRITO_EDITOR_H
