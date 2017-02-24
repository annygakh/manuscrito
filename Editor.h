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
    Editor();
    Editor(std::string);

    char getMode() {
        return m_mode;
    }

    void printBuffer();

    void updateStatus();

    void handleInput(int);

    void saveFile(std::string filename);

};


#endif //MANUSCRITO_EDITOR_H
