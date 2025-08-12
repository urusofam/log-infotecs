#include "../include/TestLogStatsCollector.h"
#include "../include/TestFramework.h"
#include "../../examples/log_stats/LogStatsCollector.h"

#include <sstream>
#include <stdexcept>

// Тестируем стандартное использование
void LogStatsCollectorTests::testLogStatsCollectorBasic() {
    LogStatsCollector stats;

    // Начальное состояние
    ASSERT_EQ(0, stats.getTotalLogs());
    ASSERT_FALSE(stats.hasChangedSinceLastPrint());

    // Добавляем лог
    stats.addLog("Test message", "INFO");

    ASSERT_EQ(1, stats.getTotalLogs());
    ASSERT_TRUE(stats.hasChangedSinceLastPrint());

    // Сбрасываем флаг изменения
    stats.resetChangeFlag();
    ASSERT_FALSE(stats.hasChangedSinceLastPrint());
}

// Тестируем с логами разных уровней
void LogStatsCollectorTests::testLogStatsCollectorMultipleLevels() {
    LogStatsCollector stats;

    stats.addLog("Error message", "ERROR");
    stats.addLog("Warning message", "WARNING");
    stats.addLog("Info message 1", "INFO");
    stats.addLog("Info message 2", "INFO");
    stats.addLog("Another error", "ERROR");

    ASSERT_EQ(5, stats.getTotalLogs());
    ASSERT_TRUE(stats.hasChangedSinceLastPrint());
}

// Тестируем сбрасывания флага изменения
void LogStatsCollectorTests::testLogStatsCollectorReset() {
    LogStatsCollector stats;

    stats.addLog("Message 1", "INFO");
    stats.addLog("Message 2", "ERROR");

    ASSERT_TRUE(stats.hasChangedSinceLastPrint());

    stats.resetChangeFlag();
    ASSERT_FALSE(stats.hasChangedSinceLastPrint());

    // Добавляем еще один лог
    stats.addLog("Message 3", "WARNING");
    ASSERT_TRUE(stats.hasChangedSinceLastPrint());
}