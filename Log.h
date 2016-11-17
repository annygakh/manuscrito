//
// Created by Anaid Gakhokidze on 2016-11-12.
//

#ifndef NCURSES_PRACTICE_LOG_H
#define NCURSES_PRACTICE_LOG_H

#include <string>
#include <iostream>
#include <fstream>

class Log {
    static Log* s_instance;
    std::string m_filename;
    FILE * m_logFile;

    Log(std::string filename = "Log_file.txt")
            : m_filename(filename)
    {
        m_logFile = fopen(m_filename.c_str(), "w");
        if (m_logFile == NULL)
        {
            std::cerr << "Error opening the file" << std::endl;
        }
    }

public:
    static Log* instance()
    {
        if (!s_instance)
        {
            s_instance = new Log();
        }
        return s_instance;
    }

    void logMessage(const char* format, ...)
    {
        va_list argptr;
        va_start(argptr, format);
        vfprintf(m_logFile, format, argptr);
        va_end(argptr);
        fflush(m_logFile);
    }
};


#endif //NCURSES_PRACTICE_LOG_H