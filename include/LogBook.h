#ifndef LOGBOOK_H
#define LOGBOOK_H

#include <string>
#include <fstream>
#include <mutex>
#include <iomanip>
#include <chrono>

// Уровни важности
enum class LogLevel
{
    INFO,
    WARNING,
    ERROR,
    ALL_LEVEL
};

// Из строки в перечисление, если передано пустое значение, то возвращает INFO
LogLevel parseFromStringToLogLevel(const std::string& levelStr);

// Из перечисления в строку
std::string logLevelToString(LogLevel level);

std::string getCurrentTime();

class LogBook
{
public:
    LogBook(const std::string& fileName, LogLevel defaultLevel);

    ~LogBook();

    // Изменение уровня по умолчанию
    void setLogLevel(LogLevel level);

    // Добавление новой строки в журнал
    void add(const std::string& message, LogLevel level);

    // Удаление операторов копирования
    LogBook(const LogBook&) = delete;
    LogBook& operator=(const LogBook&) = delete;

private:
    std::ofstream m_logFile;
    LogLevel m_currentLevel;
    std::mutex m_mutex;
};

#endif