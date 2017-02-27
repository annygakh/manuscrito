//
// Created by Anaid Gakhokidze on 2016-11-16.
//

#include "Buffer.h"
#include "Log.h"
#include <sstream>

Buffer::Buffer()
    :   m_filename("")
    , m_fileOutput(NULL)
    , m_openForWriting(false)
{
    m_lines.push_back("");
}

std::string Buffer::generateName()
{
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    std::stringstream ss;
    ss << "Untitled"
       << (now->tm_year + 1900) << '-'
       << (now->tm_mon + 1) << '-'
       <<  now->tm_mday << '_'
       << now->tm_hour << ':'
       << now->tm_sec
       << ".txt";
    return ss.str();

}

Buffer::Buffer(std::string filename)
    :   m_filename(filename)
    , m_fileOutput(NULL)
    , m_openForWriting(false)

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
        m_lines.push_back("");
        m_filename = "";
    }
    Log::instance()->logMessage("%s\n", m_filename.c_str());
    file.close();
}
bool Buffer::openFile(std::string filename)
{
    if (!m_openForWriting)
    {
        m_fileOutput.open(filename, std::ios::trunc | std::ios::out);
    }
    return true;
}

bool Buffer::saveFile()
{
    if (!m_openForWriting)
    {
        if (!filenameDefined())
        {
            m_filename = generateName();
        }
        openFile(m_filename);
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
