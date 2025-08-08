#include "FileLogger.h"

#include <iomanip>

FileLogger::FileLogger(const std::string &fileName, LogLevel level) : logFile_(fileName, std::ios::app), currentLogLevel_(level) {}

FileLogger::~FileLogger() {
    if (logFile_.is_open()) {
        logFile_.close();
    }
}

void FileLogger::log(const std::string &message, LogLevel level) {
    if (level > currentLogLevel_) {
        return;
    }

    std::lock_guard lock(mutex_);
    if (logFile_.is_open()) {
        logFile_ << message << ',' << LogLevelToString(level) << ',' << getCurrentTime() << std::endl;
    }
}

void FileLogger::setLogLevel(LogLevel level) {
    std::lock_guard lock(mutex_);
    currentLogLevel_ = level;
}

