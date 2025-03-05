#include <logger.h>
using namespace std;

Logger::Logger(const wstring& filename)
{
    // Открываем файл в Unicode (UTF-8)
    m_logFile.open(filename, ios::app);
    m_logFile.imbue(locale(locale(), new codecvt_utf8<wchar_t>()));
    if (!m_logFile.is_open()) {
        throw runtime_error("Error opening log file.");
    }
}

// Destructor: Closes the log file
Logger::~Logger() {
    if (m_logFile.is_open()) {
        m_logFile.close();
    }
}

// Logs a message with a given log level
void Logger::logg(LogLevel level, const string& message, const char* file, int line, const char* func)
{
    this->logg(level, string_to_wstring(message), string_to_wstring(file), line, string_to_wstring(func));
}
void Logger::logg(LogLevel level, const wstring& message, const char* file, int line, const char* func)
{
    this->logg(level, message, string_to_wstring(file), line, string_to_wstring(func));
}
void Logger::logg(LogLevel level, const wstring& message, const wstring& file, int line, const wstring& func)
{
    // Получаем текущее время
    time_t now = time(0);
    tm timeinfo{};
    localtime_s(&timeinfo, &now);
    wchar_t timestamp[20];
    wcsftime(timestamp, sizeof(timestamp) / sizeof(wchar_t), L"%Y-%m-%d %H:%M:%S", &timeinfo);

    // Создаём запись в логе
    wostringstream logEntry;
    logEntry << L"[" << timestamp << L"] "
        << levelToString(level) << L": " << message
        << L" (File: " << file
        << L", Line: " << line
        << L", Function: " << func << L")"
        << endl;

    // Выводим на консоль
    wcout.imbue(locale(""));  // Настраиваем локаль консоли
    wcout << logEntry.str();

    // Записываем в лог-файл
    if (m_logFile.is_open()) {
        m_logFile << logEntry.str();
        m_logFile.flush();
    }
}

void Logger::logg(LogLevel level, const char* message, const char* file, int line, const char* func)
{
    this->logg(level, string_to_wstring(message), string_to_wstring(file), line, string_to_wstring(func));
}

// Converts log level to a string for output
wstring Logger::levelToString(LogLevel level)
{
    switch (level) {
    case _DEBUG_:
        return L"DEBUG";
    case _INFO_:
        return L"INFO";
    case _WARNING_:
        return L"WARNING";
    case _ERROR_:
        return L"ERROR";
    case _CRITICAL_:
        return L"CRITICAL";
    default:
        return L"UNKNOWN";
    }
}
