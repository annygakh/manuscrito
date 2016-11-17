//
// Created by Anaid Gakhokidze on 2016-11-16.
//

#include "Buffer.h"
#include "Log.h"


Buffer::Buffer()
    : m_filename("Untitled")
    , m_file(m_filename, std::ifstream::in)
{
}

Buffer::Buffer(std::string filename)
    : m_filename(filename)
    , m_file(filename, std::ifstream::in | std::ifstream::out)
{
    Log::instance()->logMessage("Entered constructor\n");

    // TODO move the following to initialize function
    if (m_file.is_open())
    {
        while(!m_file.eof())
        {
            std::string tmp;
            getline(m_file, tmp);
            m_lines.push_back(tmp);
        }
    }
    else
    {
        Log::instance()->logMessage("Could not open file %s\n", m_filename.c_str());
    }
    Log::instance()->logMessage(m_filename.c_str());
}
