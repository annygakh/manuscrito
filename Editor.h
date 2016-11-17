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

public:
    Editor();
    Editor(std::string);
    void printBuffer();
};


#endif //MANUSCRITO_EDITOR_H
