#include "../include/LogApp.h"
#include "../include/LogBook.h"
#include <stdexcept>
#include <iostream>

int main(int argc, char* argv[]) 
{
    try 
    {
        if (argc < 3) 
        {
            throw std::invalid_argument("Using: " + std::string(argv[0]) + " <file> <level>");
        }

        LogLevel defaultLevel = parseFromStringToLogLevel(argv[2]); 
        LogApp app(argv[1], defaultLevel);
        app.start();

    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}