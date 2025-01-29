#include "../include/LogBook.h"

std::string getCurrentTime() 
{
    auto now { std::chrono::system_clock::now() };
    auto time { std::chrono::system_clock::to_time_t(now) };
    std::tm tm { *std::localtime(&time) };

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string logLevelToString(LogLevel level)
{
    switch (level)
    {
        case LogLevel::INFO:    return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR:   return "ERROR";    
        default:                return "UNKNOWN";
    }
}

LogLevel parseFromStringToLogLevel(const std::string& levelStr)
{
    if (levelStr.empty()) return LogLevel::INFO;

    if (levelStr == "INFO" || levelStr == "info" || levelStr == "Info")             return LogLevel::INFO;
    if (levelStr == "WARNING" || levelStr == "warning" || levelStr == "Warning")    return LogLevel::WARNING;
    if (levelStr == "ERROR" || levelStr == "error" || levelStr == "Error")          return LogLevel::ERROR;

    throw std::invalid_argument("Invalid level: " + levelStr + '\n');
}

LogBook::LogBook(const std::string& fileName, LogLevel defaultLevel = LogLevel::INFO)
    : m_logFile{ fileName, std::ios::app }, m_currentLevel{ defaultLevel } // Открываем файл в формате добавления
{
        if (!m_logFile.is_open())
        {
            throw std::runtime_error("Failed to open log file: " + fileName + ".\n");
        }
}

LogBook::~LogBook()
{
    if (m_logFile.is_open())
    {
        m_logFile.close();
    }
}

void LogBook::setLogLevel(LogLevel level)
{
    // Защита потока и замена уровня
    std::lock_guard<std::mutex> lock{ m_mutex };
    m_currentLevel = level;
}

void LogBook::add(const std::string& message, LogLevel level = LogLevel::INFO)
{
    if (level < m_currentLevel)
    {
        return;
    }

    std::lock_guard<std::mutex> lock{ m_mutex };

    if (!m_logFile.is_open())
    {
        throw std::runtime_error("Log file is not open.\n");
    }

    m_logFile << "[" << getCurrentTime() << "] " 
              << "[" << logLevelToString(level) << "] "
              << message << std::endl;
}