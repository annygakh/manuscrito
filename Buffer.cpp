//
// Created by Anaid Gakhokidze on 2016-11-16.
//

#include "Buffer.h"
#include "Log.h"


Buffer::Buffer()
    : m_filename("Untitled")
    , m_openForWriting(false)
    , m_fileOutput(NULL)
{
    m_lines.push_back("");
}

Buffer::Buffer(std::string filename)
    : m_filename(filename)
    , m_openForWriting(false)
    , m_fileOutput(NULL)
{
    std::ifstream file(filename, std::ifstream::out);
    Log::instance()->logMessage("Entered constructor\n");

    // TODO move the following to initialize function
    if (file.is_open())
    {
        while(!file.eof())
        {
            std::string tmp;
            getline(file, tmp);
            m_lines.push_back(tmp);
        }
    }
    else
    {
        Log::instance()->logMessage("Could not open file %s\n", m_filename.c_str());
    }
    Log::instance()->logMessage(m_filename.c_str());
    file.close();
}
bool Buffer::openFile()
{
    if (!m_openForWriting)
    {
        m_fileOutput.open(m_filename, std::ios::trunc | std::ios::out);
    }

}

bool Buffer::saveFile()
{
    if (!m_openForWriting)
    {
        openFile();
    }

    for (std::string line : m_lines)
    {
        m_fileOutput << line;
        m_fileOutput << "\n";
    }
    m_fileOutput.close();
    return true;
}

Buffer::~Buffer()
{
    if (m_openForWriting)
    {
        m_fileOutput.close();
    }
}
