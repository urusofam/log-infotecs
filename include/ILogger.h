#pragma once

#include <ctime>
#include <iomanip>
#include <string>

enum class LogLevel {
    Error,
    Warning,
    Info,
};

inline std::string LogLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::Info: return "INFO";
        case LogLevel::Warning: return "WARNING";
        case LogLevel::Error: return "ERROR";
        default: return "UNKNOWN";
    }
}


inline std::string getCurrentTime() {
    auto now = std::time(nullptr);
    std::tm tm;
    localtime_r(&now, &tm);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y %H:%M:%S");
    return oss.str();
}


class ILogger {
public:
    virtual ~ILogger() = default;

    virtual void log(const std::string &message, LogLevel level = LogLevel::Info) = 0;

    virtual void setLogLevel(LogLevel level) = 0;
};

