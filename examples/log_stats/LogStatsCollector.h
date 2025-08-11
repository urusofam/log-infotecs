#pragma once

#include <chrono>
#include <map>
#include <string>
#include <vector>

class LogStatsCollector {
    size_t totalLogs_;
    std::map<std::string, size_t> perLevel_;
    size_t lastHourLogs_;

    size_t minLength_;
    size_t maxLength_;
    double avgLength_;

    std::vector<std::chrono::system_clock::time_point> timestamps_;
    mutable bool changed_;

    void updateLastHourCount();

public:
    LogStatsCollector();

    void addLog(const std::string& message, const std::string& level);
    void print() const;
    bool hasChangedSinceLastPrint() const;
    void resetChangeFlag() const;
    size_t getTotalLogs() const;
};
