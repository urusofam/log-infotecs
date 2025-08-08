#pragma once

#include <fstream>
#include <mutex>

#include "ILogger.h"

class FileLogger : public ILogger {
    std::ofstream logFile_;
    LogLevel currentLogLevel_;
    std::mutex mutex_;

    std::string LogLevelToString(LogLevel level) const;
    std::string getCurrentTime() const;

public:
    explicit FileLogger(const std::string &fileName, LogLevel level = LogLevel::Info);
    ~FileLogger() override;

    void log(const std::string &message, LogLevel level = LogLevel::Info) override;
    void setLogLevel(LogLevel level) override;
};
