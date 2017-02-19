//
// Created by Anaid Gakhokidze on 2016-11-16.
//

#ifndef MANUSCRITO_EDITOR_H
#define MANUSCRITO_EDITOR_H

#include <string>
#include "Buffer.h"

class Editor {
private:
    Buffer * m_buffer;

    /*
     * x - exit mode
     * n - normal mode
     * i - insert mode
     * */
    char m_mode;

    int m_x, m_y;


    void handleInputInNormalMode(int);
    void handleInputInInsertMode(int);


    void handleDeleteKey();

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
};


#endif //MANUSCRITO_EDITOR_H
