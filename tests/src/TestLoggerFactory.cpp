#include "../include/TestLoggerFactory.h"
#include "../include//TestFramework.h"
#include "../../include/LoggerFactory.h"

#include <filesystem>
#include <string>

// Тестируем создание файлового логера
void LoggerFactoryTests::testCreateFileLogger() {
    const std::string filename = "test_factory_file.log";

    if (std::filesystem::exists(filename)) {
        std::filesystem::remove(filename);
    }

    auto logger = createLogger(LoggerType::FileLogger, filename, LogLevel::Info);

    ASSERT_TRUE(logger != nullptr);

    // Тестируем что логгер работает
    logger->log("Factory test message", LogLevel::Info);

    // Проверяем что файл создался
    logger.reset(); // Принудительно вызываем деструктор
    ASSERT_TRUE(std::filesystem::exists(filename));

    std::filesystem::remove(filename);
}

// Тестируем неправильное создание сокет логера
void LoggerFactoryTests::testCreateSocketLoggerInvalidTarget() {
    auto logger1 = createLogger(LoggerType::SocketLogger, "invalid", LogLevel::Info);
    ASSERT_TRUE(logger1 == nullptr);

    auto logger2 = createLogger(LoggerType::SocketLogger, "127.0.0.1", LogLevel::Info);
    ASSERT_TRUE(logger2 == nullptr);

    auto logger3 = createLogger(LoggerType::SocketLogger, "127.0.0.1:abc", LogLevel::Info);
    ASSERT_TRUE(logger3 == nullptr);
}