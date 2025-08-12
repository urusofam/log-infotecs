#include "../include/TestILogger.h"
#include "../include/TestFramework.h"
#include "../../include/ILogger.h"

#include <thread>

// Тестируем приведение уровня логирования к строке
void ILoggerTests::testLogLevelToString() {
    ASSERT_EQ("INFO", LogLevelToString(LogLevel::Info));
    ASSERT_EQ("WARNING", LogLevelToString(LogLevel::Warning));
    ASSERT_EQ("ERROR", LogLevelToString(LogLevel::Error));
}

// Тестируем получение текущегго времени
void ILoggerTests::testGetCurrentTime() {
    std::string time1 = getCurrentTime();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::string time2 = getCurrentTime();

    // Время должно быть не пустым и в правильном формате
    ASSERT_TRUE(!time1.empty());
    ASSERT_TRUE(!time2.empty());
    ASSERT_TRUE(time1.length() == time2.length());

    // Проверяем формат (должен содержать дефисы и двоеточия)
    ASSERT_TRUE(time1.find('-') != std::string::npos);
    ASSERT_TRUE(time1.find(':') != std::string::npos);
}

