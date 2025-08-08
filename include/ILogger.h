#pragma once

#include <string>

enum class LogLevel {
    Error,
    Warning,
    Info,
};

class ILogger {
public:
    virtual ~ILogger() = default;

    virtual void log(const std::string &message, LogLevel level = LogLevel::Info) = 0;

    virtual void setLogLevel(LogLevel level) = 0;
};