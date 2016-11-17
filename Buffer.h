//
// Created by Anaid Gakhokidze on 2016-11-16.
//

#ifndef MANUSCRITO_BUFFER_H
#define MANUSCRITO_BUFFER_H

#include <string>
#include <vector>
#include <fstream>

class Buffer {
private:
    std::string m_filename;
    std::ifstream m_file;

public:
    Buffer();
    Buffer(std::string);

    // TODO is this a bad practice in c++?
    std::vector<std::string> m_lines;
};


#endif //MANUSCRITO_BUFFER_H
