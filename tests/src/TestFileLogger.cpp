#include "../include/TestFileLogger.h"
#include "../../include/FileLogger.h"
#include "../include/TestFramework.h"

#include <filesystem>
#include <string>
#include <thread>

// Тестируем создание файлового логера
void FileLoggerTests::testFileLoggerCreation() {
    const std::string filename = "test_creation.log";

    // Удаляем файл если существует
    if (std::filesystem::exists(filename)) {
        std::filesystem::remove(filename);
    }

    FileLogger logger(filename, LogLevel::Info);
    logger.log("Test message", LogLevel::Info);

    // Проверяем что файл создался
    ASSERT_TRUE(std::filesystem::exists(filename));

    // Очищаем
    std::filesystem::remove(filename);
}

// Тестируем запись файлового логера
void FileLoggerTests::testFileLoggerWrite() {
    const std::string filename = "test_write.log";

    // Удаляем файл если существует
    if (std::filesystem::exists(filename)) {
        std::filesystem::remove(filename);
    }

    {
        FileLogger logger(filename, LogLevel::Info);
        logger.log("Test message 1", LogLevel::Info);
        logger.log("Test message 2", LogLevel::Error);
    } // Деструктор должен закрыть файл

    // Читаем файл и проверяем содержимое
    std::ifstream file(filename);
    ASSERT_TRUE(file.is_open());

    std::string line;
    int lineCount = 0;
    while (std::getline(file, line)) {
        if (lineCount == 0) {
            ASSERT_TRUE(line.find("Test message 1,INFO,") != std::string::npos);
        } else if (lineCount == 1) {
            ASSERT_TRUE(line.find("Test message 2,ERROR,") != std::string::npos);
        }
        lineCount++;
    }

    ASSERT_EQ(2, lineCount);

    // Очищаем
    std::filesystem::remove(filename);
}

// Тестируем уровень логирования
void FileLoggerTests::testFileLoggerLogLevel() {
    const std::string filename = "test_level.log";

    // Удаляем файл если существует
    if (std::filesystem::exists(filename)) {
        std::filesystem::remove(filename);
    }

    {
        FileLogger logger(filename, LogLevel::Warning);
        logger.log("Info message", LogLevel::Info);     // Не должно записаться
        logger.log("Warning message", LogLevel::Warning); // Должно записаться
        logger.log("Error message", LogLevel::Error);   // Должно записаться
    }

    // Читаем файл
    std::ifstream file(filename);
    std::string line;
    int lineCount = 0;
    while (std::getline(file, line)) {
        lineCount++;
        // Проверяем что Info сообщение не записалось
        ASSERT_TRUE(line.find("Info message") == std::string::npos);
    }

    ASSERT_EQ(2, lineCount); // Только Warning и Error

    // Очищаем
    std::filesystem::remove(filename);
}

// Тестируем установку уровня логирования
void FileLoggerTests::testFileLoggerSetLogLevel() {
    const std::string filename = "test_set_level.log";

    if (std::filesystem::exists(filename)) {
        std::filesystem::remove(filename);
    }

    {
        FileLogger logger(filename, LogLevel::Info);
        logger.log("Info message 1", LogLevel::Info); // Должно записаться

        logger.setLogLevel(LogLevel::Error);
        logger.log("Info message 2", LogLevel::Info); // Не должно записаться
        logger.log("Error message", LogLevel::Error); // Должно записаться
    }

    std::ifstream file(filename);
    std::string line;
    int lineCount = 0;
    bool foundInfo1 = false, foundInfo2 = false, foundError = false;

    while (std::getline(file, line)) {
        lineCount++;
        if (line.find("Info message 1") != std::string::npos) foundInfo1 = true;
        if (line.find("Info message 2") != std::string::npos) foundInfo2 = true;
        if (line.find("Error message") != std::string::npos) foundError = true;
    }

    ASSERT_TRUE(foundInfo1);
    ASSERT_FALSE(foundInfo2);
    ASSERT_TRUE(foundError);
    ASSERT_EQ(2, lineCount);

    std::filesystem::remove(filename);
}

// Тестируем потокобезопасность
void FileLoggerTests::testFileLoggerThreadSafety() {
    const std::string filename = "test_thread_safety.log";

    if (std::filesystem::exists(filename)) {
        std::filesystem::remove(filename);
    }

    constexpr int numThreads = 4;
    constexpr int messagesPerThread = 10;

    {
        FileLogger logger(filename, LogLevel::Info);

        // Запускаем несколько потоков для записи
        std::vector<std::thread> threads;


        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back([&logger, i]() {
                for (int j = 0; j < messagesPerThread; ++j) {
                    logger.log("Thread " + std::to_string(i) + " Message " + std::to_string(j), LogLevel::Info);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
            });
        }

        for (auto& thread : threads) {
            thread.join();
        }
    }

    // Проверяем количество строк в файле
    std::ifstream file(filename);
    std::string line;
    int lineCount = 0;
    while (std::getline(file, line)) {
        lineCount++;
    }

    ASSERT_EQ(numThreads * messagesPerThread, lineCount);

    std::filesystem::remove(filename);
}