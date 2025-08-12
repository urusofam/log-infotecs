#include "include/TestFileLogger.h"
#include "include/TestFramework.h"
#include "include/TestILogger.h"
#include "include/TestLoggerFactory.h"
#include "include/TestLogStatsCollector.h"
#include "include/TestThreadSafeQueue.h"

int main() {
    TestFramework framework;

    // FileLogger тесты
    framework.addTest("Создание FileLogger", FileLoggerTests::testFileLoggerCreation);
    framework.addTest("Запись FileLogger", FileLoggerTests::testFileLoggerWrite);
    framework.addTest("Уровень логирования FileLogger", FileLoggerTests::testFileLoggerLogLevel);
    framework.addTest("Установка уровня логирования FileLogger", FileLoggerTests::testFileLoggerSetLogLevel);
    framework.addTest("Потокобезопасность FileLogger", FileLoggerTests::testFileLoggerThreadSafety);

    // LogStatsCollector тесты
    framework.addTest("Обычное использование LogStatsCollector", LogStatsCollectorTests::testLogStatsCollectorBasic);
    framework.addTest("Несколько уровней логирования LogStatsCollector", LogStatsCollectorTests::testLogStatsCollectorMultipleLevels);
    framework.addTest("Сброс LogStatsCollector", LogStatsCollectorTests::testLogStatsCollectorReset);

    // ThreadSafeQueue тесты
    framework.addTest("Обычное использование ThreadSafeQueue", ThreadSafeQueueTests::testThreadSafeQueueBasic);
    framework.addTest("Пустая ThreadSafeQueue", ThreadSafeQueueTests::testThreadSafeQueueEmpty);
    framework.addTest("Потокобезопасность ThreadSafeQueue", ThreadSafeQueueTests::testThreadSafeQueueThreadSafety);

    // LoggerFactory тесты
    framework.addTest("Создание файлового логера LoggerFactory", LoggerFactoryTests::testCreateFileLogger);
    framework.addTest("Неправильное создание сокет логера LoggerFactory", LoggerFactoryTests::testCreateSocketLoggerInvalidTarget);

    // ILogger тесты
    framework.addTest("Приведение уровня логирования к строке", ILoggerTests::testLogLevelToString);
    framework.addTest("Получение текущего времени", ILoggerTests::testGetCurrentTime);


    framework.runAllTests();

    return framework.getFailedCount() == 0 ? 0 : 1;
}
