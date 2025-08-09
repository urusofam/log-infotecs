#include "FileLogger.h"

#include <iomanip>

// Конструктор для файлового логера
FileLogger::FileLogger(const std::string &fileName, LogLevel level) : logFile_(fileName, std::ios::app), currentLogLevel_(level) {}

// Деструктор для файлового логера
FileLogger::~FileLogger() {
    if (logFile_.is_open()) {
        logFile_.close();
    }
}

// Функция логирования для фалового логера
void FileLogger::log(const std::string &message, LogLevel level) {
    // Если уровень переданного сообщения ниже текущего
    if (level > currentLogLevel_) {
        return;
    }

    // Пишем в файл
    std::lock_guard lock(mutex_);
    if (logFile_.is_open()) {
        logFile_ << message << ',' << LogLevelToString(level) << ',' << getCurrentTime() << std::endl;
    }
}

// Функция установки уровня логирования
void FileLogger::setLogLevel(LogLevel level) {
    std::lock_guard lock(mutex_);
    currentLogLevel_ = level;
}

