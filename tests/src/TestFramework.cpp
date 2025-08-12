#include "../include/TestFramework.h"

#include <iostream>

// Конструктор для TestFramework
TestFramework::TestFramework() : passed_(0), failed_(0) {}

// Функция добавления теста
void TestFramework::addTest(const std::string &name, const std::function<void()> &test) {
    tests_.push_back({name, test});
}

// Функция запуска всех тестов
void TestFramework::runAllTests() {
    std::cout << "Всего тестов: " << tests_.size() << std::endl;

    for (const auto&[name, test] : tests_) {
        try {
            std::cout << "Запускаем тест: " << name << "... " << std::endl;
            test();
            std::cout << "ТЕСТ ПРОЙДЕН" << std::endl;
            passed_++;
        } catch (const std::exception& e) {
            std::cout << "ТЕСТ НЕ ПРОЙДЕН: " << e.what() << std::endl;
            failed_++;
        }
    }

    std::cout << std::endl;
    std::cout << "Результаты тестирования: " << passed_ << " тестов пройдено, " << failed_ << " тестов не пройдено" << std::endl;

    if (failed_ == 0) {
        std::cout << "ВСЕ ТЕСТЫ ПРОЙДЕНЫ" << std::endl;
    }
}

// Получение количества непройденных тестов
int TestFramework::getFailedCount() const {
    return failed_;
}


