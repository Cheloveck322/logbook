#include "../include/LogApp.h"
#include <iostream>

LogApp::LogApp(const std::string& fileName, LogLevel defaultLevel)
    : m_logbook{ fileName, defaultLevel }
{
}

void LogApp::start()
{
    std::string input{};
    std::string strLevel{};

    while(m_isRunning)
    {
        try
        {
            std::cout << "Enter message or command \"setlevel <LEVEL>\" (or \"exit\" to stop): ";
            std::getline(std::cin, input);

            // Обработка выхода 
            if (input == "exit" || input == "exit ")
            {
                stop();
                break;
            }

            // Обработка изменения режима по умолчанию
            if (input.find("setlevel ") == 0)
            {
                m_changeDefaultLevel(input);
                continue;
            }

            std::cout << "Enter level of the message (INFO, WARNING, ERROR, default is INFO): ";
            std::getline(std::cin, strLevel);

            LogLevel level = parseFromStringToLogLevel(strLevel);

            // Запуск потока для записи
            std::thread([this, input, level]()
                {
                    std::lock_guard<std::mutex> lock{ m_mutex };
                    m_logbook.add(input, level);
                }).detach();

        }
        catch(const std::exception& e)
        {
            m_printError(e.what());
        }
        
    }
}

void LogApp::stop()
{
    m_isRunning = false;
    std::cout << "\nShut down..." << std::endl;
}

void LogApp::m_printError(const std::string& input)
{
    std::lock_guard<std::mutex> lock{ m_mutex };
    std::cerr << "[ERROR] " << input << std::endl;
}

void LogApp::m_changeDefaultLevel(const std::string& input)
{
    size_t pos { input.find(' ') };

    if (pos == std::string::npos) 
    {
        throw std::invalid_argument("Invalid command format.\n");
    }

    std::string levelStr { input.substr(pos + 1) };
    LogLevel newLevel;

    if (levelStr.empty())
    {
        newLevel = LogLevel::INFO;
    }
    else
    {
        newLevel = parseFromStringToLogLevel(levelStr);
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    m_logbook.setLogLevel(newLevel);
    

    std::cout << "Default level changed to : " << (levelStr.empty() ? "INFO" : levelStr) << std::endl;
}