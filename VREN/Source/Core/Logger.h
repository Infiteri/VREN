#pragma once

#include "Base.h"
#include <string>
#include <memory>

namespace VREN
{
    enum class LogLevel
    {
        Info,
        Debug,
        Warn,
        Error,
    };

    class VREN_API Logger
    {
    public:
        Logger(const std::string &name);

        virtual void Log(LogLevel level, const char* msg, ...) = 0;

    public:
        static void SetCurrentLogger(std::shared_ptr<Logger> logger);
        static std::shared_ptr<Logger> GetActiveLogger();

    private:
    protected:
        std::string name;
    };

    /// @brief The CoreLogger (aka the default logger) will only log the errors and warnings
    class VREN_API CoreLogger : public Logger
    {
    public:
        CoreLogger(const std::string &name);

        void Log(LogLevel level, const char* msg, ...);
    };
}

#define VREN_LOG(lvl, msg, ...)                                            \
    {                                                                      \
        if (VREN::Logger::GetActiveLogger())                                \
            VREN::Logger::GetActiveLogger()->Log(lvl, msg, ##__VA_ARGS__); \
    }
