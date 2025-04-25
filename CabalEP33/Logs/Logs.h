#pragma once
#include <Windows.h>
#include <memory>
#include <string>
#include <iostream>
#include <format>

enum LogLevel
{
    INFO,
    WARNING,
    ERRORS
};

class Log
{
public:
    std::string getTimeStamp()
    {
        std::time_t now = std::time(nullptr);
        std::tm tm;
        localtime_s(&tm, &now);
        char buffer[20];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);
        return std::string(buffer);
    }

    std::string getLogLevelString(LogLevel level)
    {
        switch (level)
        {
        case INFO:
            return "[INFO]";
        case WARNING:
            return "[WARNING]";
        case ERRORS:
            return "[ERROR]";
        default:
            return "[UNKNOWN]";
        }
    }

    void WriteLog(LogLevel level, std::string message)
    {
#ifdef DEV_MODE
        std::string levelString = getLogLevelString(level);
        std::string colorCode = "";
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        switch (level)
        {
        case INFO:
            SetConsoleTextAttribute(console, FOREGROUND_BLUE | FOREGROUND_GREEN);
            break;
        case WARNING:
            SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            break;
        case ERRORS:
            SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY);
            break;
        }
        std::cout << levelString << "[" << getTimeStamp() << "]" << ":" << " ";

        std::cout << message << std::endl;

        SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#endif
    }
};

extern std::shared_ptr<Log> g_pLogs;

#define LOG_INFO(msg,...)       g_pLogs->WriteLog(INFO,std::format(msg,__VA_ARGS__))
#define LOG_ERROR(msg,...)      g_pLogs->WriteLog(ERRORS,std::format(msg,__VA_ARGS__))
#define LOG_WARNING(msg,...)    g_pLogs->WriteLog(WARNING,std::format(msg,__VA_ARGS__))

