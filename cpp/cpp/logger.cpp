#include <logger.h>
using namespace std;

Logger::Logger(const string& filename)
{
    logFile.open(filename, ios::app);
    if (!logFile.is_open()) {
        cerr << "Error opening log file." << endl;
    }
}

// Destructor: Closes the log file
Logger::~Logger() { logFile.close(); }

// Logs a message with a given log level
void Logger::logg(LogLevel level, const string& message, const char* file, int line, const char* func)
{
    // Get current timestamp
    time_t now = time(0);
    tm* timeinfo = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp),
        "%Y-%m-%d %H:%M:%S", timeinfo);

    // Create log entry
    ostringstream logEntry;
    logEntry << "[" << timestamp << "] "
        << levelToString(level) << ": " << message
        << " (File: " << file
        << ", Line: " << line
        << ", Function: " << func << ")"
        << endl;

    // Output to console
    cout << logEntry.str();

    // Output to log file
    if (logFile.is_open()) {
        logFile << logEntry.str();
        logFile.flush(); // Ensure immediate write to file
    }
}

void Logger::logg(LogLevel level, const char* message, const char* file, int line, const char* func)
{
    // Get current timestamp
    time_t now = time(0);
    tm* timeinfo = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp),
        "%Y-%m-%d %H:%M:%S", timeinfo);

    // Create log entry
    ostringstream logEntry;
    logEntry << "[" << timestamp << "] "
        << levelToString(level) << ": " << message
        << " (File: " << file
        << ", Line: " << line
        << ", Function: " << func << ")"
        << endl;

    // Output to console
    cout << logEntry.str();

    // Output to log file
    if (logFile.is_open()) {
        logFile << logEntry.str();
        logFile.flush(); // Ensure immediate write to file
    }
}

// Converts log level to a string for output
string Logger::levelToString(LogLevel level)
{
    switch (level) {
    case _DEBUG_:
        return "DEBUG";
    case _INFO_:
        return "INFO";
    case _WARNING_:
        return "WARNING";
    case _ERROR_:
        return "ERROR";
    case _CRITICAL_:
        return "CRITICAL";
    default:
        return "UNKNOWN";
    }
}
