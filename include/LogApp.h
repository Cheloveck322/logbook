#ifndef LOGAPP_H
#define LOGAPP_H

#include "LogBook.h"
#include <thread>
#include <memory>
#include <atomic>

class LogApp
{
public:
    LogApp(const std::string& fileName, LogLevel defaultLevel);

    void start();

    void stop();

private:
    LogBook m_logbook;
    std::mutex m_mutex;
    std::atomic<bool> m_isRunning{ true }; // Флаг для управления циклом

    // Вывод ошибки с защитой потока
    void m_printError(const std::string& message);

    // Изменение уровня важности по умолчанию
    void m_changeDefaultLevel(const std::string& input);
};

#endif