#include "../include/TestThreadSafeQueue.h"
#include "../../examples/log_writer/ThreadSafeQueue.h"
#include "../include/TestFramework.h"

#include <atomic>
#include <thread>

void ThreadSafeQueueTests::testThreadSafeQueueBasic() {
    ThreadSafeQueue queue;

    Log log1{"Test message 1", LogLevel::Info};
    Log log2{"Test message 2", LogLevel::Error};

    // Добавляем логи
    queue.push(log1);
    queue.push(log2);

    // Извлекаем логи
    Log retrieved1, retrieved2;
    ASSERT_TRUE(queue.pop(retrieved1));
    ASSERT_TRUE(queue.pop(retrieved2));

    ASSERT_EQ(log1.message, retrieved1.message);
    ASSERT_EQ(log2.message, retrieved2.message);
}

void ThreadSafeQueueTests::testThreadSafeQueueEmpty() {
    ThreadSafeQueue queue;

    // Останавливаем очередь
    queue.stop();

    Log log;
    // Должно вернуть false для пустой остановленной очереди
    ASSERT_FALSE(queue.pop(log));
}

void ThreadSafeQueueTests::testThreadSafeQueueThreadSafety() {
    ThreadSafeQueue queue;
    constexpr int numProducers = 2;
    constexpr int numConsumers = 2;
    constexpr int messagesPerProducer = 50;

    std::atomic<int> consumedCount{0};
    std::vector<std::thread> threads;

    // Producer threads
    for (int i = 0; i < numProducers; ++i) {
        threads.emplace_back([&queue, i]() {
            for (int j = 0; j < messagesPerProducer; ++j) {
                Log log{"Producer " + std::to_string(i) + " Message " + std::to_string(j), LogLevel::Info};
                queue.push(log);
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            }
        });
    }

    // Consumer threads
    for (int i = 0; i < numConsumers; ++i) {
        threads.emplace_back([&queue, &consumedCount]() {
            Log log;
            while (queue.pop(log)) {
                ++consumedCount;
            }
        });
    }

    // Ждем завершения producers
    for (int i = 0; i < numProducers; ++i) {
        threads[i].join();
    }

    // Небольшая задержка для обработки оставшихся сообщений
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Останавливаем очередь
    queue.stop();

    // Ждем завершения consumers
    for (int i = numProducers; i < numProducers + numConsumers; ++i) {
        threads[i].join();
    }

    // Проверяем что все сообщения обработаны
    ASSERT_EQ(numProducers * messagesPerProducer, consumedCount.load());
}
