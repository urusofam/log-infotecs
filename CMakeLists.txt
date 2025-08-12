cmake_minimum_required(VERSION 3.21)
project(log_infotecs CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Основная библиотека логгера
add_library(loggerLib
        src/FileLogger.cpp
        src/SocketLogger.cpp
        src/LoggerFactory.cpp
)
target_include_directories(loggerLib PUBLIC include)
target_link_libraries(loggerLib PUBLIC pthread)

# log_writer
set(LOG_WRITER_DIR examples/log_writer)
add_library(log_writer_lib ${LOG_WRITER_DIR}/ThreadSafeQueue.cpp)
target_include_directories(log_writer_lib PUBLIC ${LOG_WRITER_DIR})
target_link_libraries(log_writer_lib PUBLIC loggerLib)

add_executable(log_writer ${LOG_WRITER_DIR}/log_writer.cpp)
target_link_libraries(log_writer PRIVATE log_writer_lib)

# log_stats
set(LOG_STATS_DIR examples/log_stats)
add_library(log_stats_lib
        ${LOG_STATS_DIR}/LogStatsCollector.cpp
        ${LOG_STATS_DIR}/LogServer.cpp
)
target_include_directories(log_stats_lib PUBLIC ${LOG_STATS_DIR})
target_link_libraries(log_stats_lib PUBLIC loggerLib)

add_executable(log_stats ${LOG_STATS_DIR}/log_stats.cpp)
target_link_libraries(log_stats PRIVATE log_stats_lib)

# Тесты
enable_testing()

file(GLOB TEST_SOURCES CONFIGURE_DEPENDS tests/src/*.cpp)

add_library(testLib ${TEST_SOURCES})
target_include_directories(testLib
        PUBLIC
        tests/include
        ${LOG_WRITER_DIR}
        ${LOG_STATS_DIR}
)
target_link_libraries(testLib
        PUBLIC
        loggerLib
        log_writer_lib
        log_stats_lib
)

add_executable(unit_tests tests/main.cpp)
target_link_libraries(unit_tests PRIVATE testLib)

add_test(NAME UnitTests COMMAND unit_tests)