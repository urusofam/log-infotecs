#include "LoggerFactory.h"
#include "SocketLogger.h"

// Функция создания логера через фабрику
std::unique_ptr<ILogger> createLogger(LoggerType type, const std::string &target, LogLevel level) {
    // Если файловый логер
    if (type == LoggerType::FileLogger) {
        return std::make_unique<FileLogger>(target, level);
    }
    // Если сокет логер
    if (type == LoggerType::SocketLogger) {
        // Находим :
        size_t colon_pos = target.find(':');
        if (colon_pos == std::string::npos) {
            return nullptr;
        }

        // Получаем ip и порт
        std::string ip = target.substr(0, colon_pos);
        int port;
        try {
            port = std::stoi(target.substr(colon_pos + 1));
        } catch (...) {
            return nullptr;
        }

        // Создаем сокет логер
        auto loger = std::make_unique<SocketLogger>(ip, port, level);
        if (!loger->connectToServer()) {
            return nullptr;
        }
        return loger;
    }

    // Если несуществующий
    return nullptr;
}
