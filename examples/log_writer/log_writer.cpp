#include <atomic>
#include <iostream>
#include <thread>

#include "LoggerFactory.h"
#include "SocketLogger.h"
#include "ThreadSafeQueue.h"

// Парсер типа логера
bool parseLoggerType(LoggerType& type, const std::string& typeStr) {
    if (typeStr == "file") {
        type = LoggerType::FileLogger;
        return true;
    }
    if (typeStr == "socket") {
        type = LoggerType::SocketLogger;
        return true;
    }
    return false;
}

// Парсер уровня логирования
bool parseLogLevel(LogLevel& level, const std::string& levelStr) {
    if (levelStr == "error") {
        level = LogLevel::Error;
        return true;
    }
    if (levelStr == "warning") {
        level = LogLevel::Warning;
        return true;
    }
    if (levelStr == "info") {
        level = LogLevel::Info;
        return true;
    }
    return false;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cerr << "Использование: " << argv[0] << " <file|socket> <filename|ip:port> <LogLevel>" << std::endl;
        return 1;
    }

    // Получаем значения
    std::string typeStr = argv[1];
    std::string target = argv[2];
    std::string levelStr = argv[3];

    // Парсим тип логера
    LoggerType type;
    if (!parseLoggerType(type, typeStr)) {
        std::cerr << "Неизвестный тип логера: " << typeStr << std::endl;
        return 1;
    }

    // Парсим уровень логирования
    LogLevel level;
    if (!parseLogLevel(level, levelStr)) {
        std::cerr << "Неизвестный уровень логирования: " << levelStr << std::endl;
        return 1;
    }

    // Создаем логер
    auto logger = createLogger(type, target, level);
    if (!logger) {
        std::cerr << "Не удалось создать логер";
        return 1;
    }

    // Создаем очередь
    ThreadSafeQueue queue;
    std::atomic<bool> running(true);

    // Запускаем поток чтения логов из очереди
    std::thread thread([&]() {
        Log log;
        while (queue.pop(log)) {
            logger->log(log.message, log.level);
        }
        running = false;
    });

    // Запускаем чтение логов из консоли
    std::cout << "Вводите сообщения (q чтобы выйти):" << std::endl;
    std::string input;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, input)) break;
        if (input == "q") break;

        // Уровень должен быть задан в начале строки: "[warning] text"
        LogLevel lvl = level;
        if (input.rfind("[error]", 0) == 0) {
            lvl = LogLevel::Error;
            input = input.substr(8);
        } else if (input.rfind("[warning]", 0) == 0) {
            lvl = LogLevel::Warning;
            input = input.substr(10);
        } else if (input.rfind("[info]", 0) == 0) {
            lvl = LogLevel::Info;
            input = input.substr(7);
        }

        queue.push({input, lvl});
    }

    // Останавливаем программу
    queue.stop();
    thread.join();

    return 0;
}
