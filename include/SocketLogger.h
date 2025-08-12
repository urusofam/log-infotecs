#pragma once

#include <mutex>

#include "ILogger.h"

class SocketLogger : public ILogger {
    int socket_;
    std::string serverIP_;
    int serverPort_;
    LogLevel currentLogLevel_;
    std::mutex mutex_;

public:
    explicit SocketLogger(std::string serverIP, int serverPort, LogLevel logLevel = LogLevel::Info);
    ~SocketLogger() override;

    void log(const std::string& message, LogLevel level) override;
    void setLogLevel(LogLevel level) override;
    bool connectToServer();
};
