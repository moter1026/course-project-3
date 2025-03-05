// C++ program to implement a basic logging system.
#pragma once
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Classes_for_files.h"
using namespace std;

#pragma once

// Макрос для логирования
#define logging(level, message) Logger::logg(level, message, __FILE__, __LINE__, __func__)

// Enum to represent log levels
enum LogLevel { _DEBUG_, _INFO_, _WARNING_, _ERROR_, _CRITICAL_ };

class Logger {
public:
    // Constructor: Opens the log file in append mode
    Logger(const wstring& filename);

    // Destructor: Closes the log file
    ~Logger();
    // Logs a message with a given log level
    void logg(LogLevel level,
            const string& message,
            const char* file,
            int line,
            const char* func);
    void logg(LogLevel level,
        const wstring& message,
        const char* file,
        int line,
        const char* func);
    void logg(LogLevel level,
        const wstring& message,
        const wstring& file,
        int line,
        const wstring& func);
    void logg(LogLevel level,
        const char* message,
        const char* file,
        int line,
        const char* func);

private:
    wofstream m_logFile; // File stream for the log file

    // Converts log level to a string for output
    wstring levelToString(LogLevel level);
};