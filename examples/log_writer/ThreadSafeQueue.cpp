#include "ThreadSafeQueue.h"

// Конструктор для потокобезопасной очереди
ThreadSafeQueue::ThreadSafeQueue() : stopped_(false) {}

// Функция добавления лога в очередь
void ThreadSafeQueue::push(Log log) {
    std::unique_lock lock(mutex_);
    logs_.push(log);
    cv_.notify_one();
}

// Функция получения лога из очереди
bool ThreadSafeQueue::pop(Log& log) {
    std::unique_lock lock(mutex_);

    // Ждём пока очередь станет не пустой или пока не остановят программу
    cv_.wait(lock, [this] { return !logs_.empty() || stopped_; });
    if (logs_.empty()) return false;

    log = std::move(logs_.front());
    logs_.pop();
    return true;
}

// Функция уведомления о завершении программы
void ThreadSafeQueue::stop() {
    std::unique_lock lock(mutex_);
    stopped_ = true;
    cv_.notify_all();
}