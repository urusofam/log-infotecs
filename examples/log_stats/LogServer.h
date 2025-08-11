#pragma once

#include <atomic>
#include <string>

#include "LogStatsCollector.h"

class LogServer {
    std::string ip_;
    int port_;
    LogStatsCollector& stats_;
    size_t N_;
    std::atomic<bool>& running_;

public:
    LogServer(std::string  ip, int port, LogStatsCollector& stats, size_t N, std::atomic<bool>& running);
    void run() const;
};
