#include "LoggerFactory.h"
#include "SocketLogger.h"

#include <sstream>

// Функция создания логера через фабрику
std::unique_ptr<ILogger> createLogger(LoggerType type, const std::string &fileName, LogLevel level) {
    // Если файловый логер
    if (type == LoggerType::FileLogger) {
        return std::make_unique<FileLogger>(fileName, level);
    }

    // Если сокет логер
    std::istringstream ss(fileName);
    std::string ip;
    int port;
    char colon;
    if (ss >> ip >> colon >> port && colon == ':') {
        return std::make_unique<SocketLogger>(ip, port, level);
    }

    // Если несуществующий
    return nullptr;
}
