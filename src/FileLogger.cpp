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

std::string FileLogger::LogLevelToString(LogLevel level) const {
    switch (level) {
        case LogLevel::Info: return "INFO";
        case LogLevel::Warning: return "WARNING";
        case LogLevel::Error: return "ERROR";
        default: return "UNKNOWN";
    }
}


std::string FileLogger::getCurrentTime() const {
    auto now = std::time(nullptr);
    std::tm tm;
    localtime_r(&now, &tm);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y %H:%M:%S");
    return oss.str();
}


