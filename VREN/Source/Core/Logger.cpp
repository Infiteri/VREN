#include "Logger.h"

#include <cstdarg>
#include <iostream>
#include <string>

namespace VREN
{
    static const char *const levelString[] = {"Info", "Debug", "Warn", "Error"};
    static std::shared_ptr<Logger> activeLogger = std::make_shared<CoreLogger>("VREN");

    enum class LogColor
    {
        Green,
        Blue,
        Yellow,
        Red,
        White
    };

    struct RGB
    {
        int r, g, b;
    };

#ifdef _WIN32
#include <windows.h>
#endif

    static void EnableAnsiColors()
    {
#ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE)
            return;

        DWORD dwMode = 0;
        if (!GetConsoleMode(hOut, &dwMode))
            return;

        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
#endif
    }

    static RGB LogColorToRGB(LogColor color)
    {

        switch (color)
        {
        case LogColor::Red:
            return {255, 50, 50};
        case LogColor::Green:
            return {50, 255, 50};
        case LogColor::Yellow:
            return {255, 255, 0};
        case LogColor::Blue:
            return {50, 50, 255};
        case LogColor::White:
            return {255, 255, 255};
        default:
            return {255, 255, 255};
        }
    }

    static void SetLogColor(LogColor color)
    {
        RGB c = LogColorToRGB(color);
        std::cout << "\033[38;2;" << c.r << ";" << c.g << ";" << c.b << "m";
    }

    CoreLogger::CoreLogger(const std::string &name) : Logger(name)
    {
        EnableAnsiColors();
    }

    static void OutputMessage(const char *message)
    {
        std::cout << message << "\033[0m\n"; // reset color
    }

    void CoreLogger::Log(LogLevel level, const char *msg, ...)
    {
        if (level == LogLevel::Info || level == LogLevel::Debug)
            return;

        const char *strLevel = levelString[static_cast<int>(level)];
        LogColor color = (LogColor)level;

        char message[32000];

        va_list args;
        va_start(args, msg);
        int ret = std::vsnprintf(message, 32000, msg, args);
        va_end(args);

        if (ret < 0)
            return;

        char finalMsg[32005];
        std::snprintf(finalMsg, 32005, "[%s %s]: %s", name.c_str(), strLevel, message);

        SetLogColor(color);
        OutputMessage(finalMsg);
    }

    Logger::Logger(const std::string &name) : name(name)
    {
    }

    void Logger::SetCurrentLogger(std::shared_ptr<Logger> logger)
    {
        activeLogger = logger;
    }

    std::shared_ptr<Logger> Logger::GetActiveLogger()
    {
        return activeLogger;
    }
}