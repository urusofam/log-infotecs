#pragma once

#include <memory>

#include "FileLogger.h"

enum class LoggerType {
    FileLogger,
    SocketLogger,
};

std::unique_ptr<ILogger> createLogger(LoggerType type, const std::string &fileName, LogLevel level);