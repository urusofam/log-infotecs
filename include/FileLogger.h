#pragma once

#include <fstream>
#include <mutex>

#include "ILogger.h"

class FileLogger : public ILogger {
    std::ofstream logFile_;
    LogLevel currentLogLevel_;
    std::mutex mutex_;

public:
    explicit FileLogger(const std::string &fileName, LogLevel level = LogLevel::Info);
    ~FileLogger() override;

    void log(const std::string &message, LogLevel level) override;
    void setLogLevel(LogLevel level) override;
};
