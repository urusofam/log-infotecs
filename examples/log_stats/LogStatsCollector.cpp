#include "LogStatsCollector.h"

#include <algorithm>
#include <iostream>

// Конструктор для сборщика статистики логов
LogStatsCollector::LogStatsCollector() : totalLogs_(0), lastHourLogs_(0),
    minLength_(SIZE_MAX), maxLength_(0), avgLength_(0.0), changed_(false) {}

// Добавляем лог в статистику
void LogStatsCollector::addLog(const std::string& message, const std::string& level) {
    // Увеличиваем общие счетчики
    totalLogs_++;
    perLevel_[level]++;

    // Добавляем временную метку и обновляем счетчик за последний час
    timestamps_.push_back(std::chrono::system_clock::now());
    updateLastHourCount();

    // Обновляем статистику по длине сообщения
    size_t len = message.size();
    minLength_ = std::min(minLength_, len);
    maxLength_ = std::max(maxLength_, len);
    avgLength_ += (static_cast<double>(len) - avgLength_) / static_cast<double>(totalLogs_);

    // Устанавливаем флаг, что данные изменились
    changed_ = true;
}

// Пересчитываем сообщения за последний час
void LogStatsCollector::updateLastHourCount() {
    auto now = std::chrono::system_clock::now();
    auto oneHourAgo = now - std::chrono::hours(1);

    timestamps_.erase(
        std::remove_if(timestamps_.begin(), timestamps_.end(),
                       [&](auto& tp) { return tp < oneHourAgo; }),
        timestamps_.end()
    );
    lastHourLogs_ = timestamps_.size();
}

// Печатаем статистику
void LogStatsCollector::print() const {
    std::cout << std::endl << "=== Статистика ===" << std::endl;
    std::cout << "Всего лого: " << totalLogs_ << std::endl;

    std::cout << "Логи по уровням:" << std::endl;
    for (auto& p : perLevel_) {
        std::cout << " - " << p.first << ": " << p.second << std::endl;
    }

    std::cout << "Логи за последний час: " << lastHourLogs_ << std::endl;

    if (minLength_ != SIZE_MAX) {
        std::cout << "Длина сообщения (мин/макс/средн): "
                  << minLength_ << " / "
                  << maxLength_ << " / "
                  << avgLength_ << "\n";
    }

    std::cout << "==================" << std::endl;
}

// Поменялась ли статистика
bool LogStatsCollector::hasChangedSinceLastPrint() const {
    return changed_;
}

// Сбросить флаг изменения
void LogStatsCollector::resetChangeFlag() const {
    changed_ = false;
}

// Количество всех логов
size_t LogStatsCollector::getTotalLogs() const {
    return totalLogs_;
}