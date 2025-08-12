#pragma once

#include <functional>
#include <string>

#define ASSERT_TRUE(condition) \
    if (!(condition)) { \
        std::ostringstream oss; \
        oss << #condition "должно быть true в строке " << __LINE__; \
        throw std::runtime_error(oss.str()); \
    }

#define ASSERT_FALSE(condition) \
    if (condition) { \
        std::ostringstream oss; \
        oss << #condition " должно быть false в строке " << __LINE__; \
        throw std::runtime_error(oss.str()); \
    }

#define ASSERT_EQ(expected, actual) \
    if ((expected) != (actual)) { \
        std::ostringstream oss; \
        oss << "Ожидалось " << (expected) << " но получилось " << (actual) << " в строке " << __LINE__; \
        throw std::runtime_error(oss.str()); \
    }

#define ASSERT_NE(expected, actual) \
    if ((expected) == (actual)) { \
        std::ostringstream oss; \
        oss << (expected) << " не должен быть равен " << (actual) << " в строке " << __LINE__; \
        throw std::runtime_error(oss.str()); \
    }

#define ASSERT_THROWS(expression, exceptionType) \
    { \
        bool caught = false; \
        try { \
            expression; \
        } catch (const exceptionType&) { \
            caught = true; \
        } catch (...) { \
            std::ostringstream oss; \
            oss << "Неправильный тип exception выброшен в строке " << __LINE__; \
            throw std::runtime_error(oss.str()); \
        } \
        if (!caught) { \
            std::ostringstream oss; \
            oss << "Ожидаемый exception " #exceptionType " не был выброшен в строке " << __LINE__; \
            throw std::runtime_error(oss.str()); \
        } \
    }

class TestFramework {
    struct TestCase {
        std::string name;
        std::function<void()> test;
    };

    std::vector<TestCase> tests_;
    int passed_;
    int failed_;

    public:
    TestFramework();

    void addTest(const std::string& name, const std::function<void()> &test);
    void runAllTests();

    [[nodiscard]] int getFailedCount() const;
};
