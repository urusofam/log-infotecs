#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>

#include "ILogger.h"

struct Log {
    std::string message;
    LogLevel level;
};

class ThreadSafeQueue {
    std::queue<Log> logs_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool stopped_;

public:
    ThreadSafeQueue();

    void push(Log log);
    bool pop(Log& log);
    void stop();
};
