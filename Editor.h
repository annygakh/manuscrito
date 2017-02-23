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

    int m_x, m_y;
    int m_prevX, m_prevY;

    // When the user writes anything while being prompted, it appears here
    std::string m_commandSoFar;

    Action m_pendingAction;


    void handleInputInNormalMode(int);
    void handleInputInInsertMode(int);
    void handleInputInPromptMode(int);
    std::string handlePrompt();

    void handleDeleteKey();
    void handleEnterKey();

    void handleMoveUpNormalInsert();
    void handleMoveDownNormalInsert();
    void handleMoveLeftNormalInsert();
    void handleDeleteKeyInsertMode();
    void handleEnterKeyInsertMode();
    void handleEnterKeyPromptMode();

    void moveDown();
    void moveUp();
    void moveLeft();
    void moveRight();

public:
    Editor();
    Editor(std::string);

    char getMode() {
        return m_mode;
    }

    void printBuffer();

    void updateStatus();

    void handleInput(int);

    void saveFile(std::string filename);


    void handleMoveRightNormalInsert();
};


#endif //MANUSCRITO_EDITOR_H
