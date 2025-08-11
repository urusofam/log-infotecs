cmake_minimum_required(VERSION 3.21)
project(log_infotecs CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

set(LOGGER_SOURCES
        src/FileLogger.cpp
        src/SocketLogger.cpp
        src/LoggerFactory.cpp
)

add_library(loggerLib ${LOGGER_SOURCES})
target_include_directories(loggerLib PUBLIC include)
target_link_libraries(loggerLib PUBLIC pthread)

SET(LOG_WRITER_DIR examples/log_writer)

add_executable(log_writer ${LOG_WRITER_DIR}/log_writer.cpp ${LOG_WRITER_DIR}/ThreadSafeQueue.cpp)
target_include_directories(log_writer PRIVATE examples/log_writer)
target_link_libraries(log_writer PRIVATE loggerLib)

SET(LOG_STATS_SOURCES
        examples/log_stats/LogServer.cpp
        examples/log_stats/LogStatsCollector.cpp
)
set(LOG_STATS_DIR examples/log_stats)

add_library(logStatsLib ${LOG_STATS_SOURCES})
target_include_directories(logStatsLib PRIVATE ${LOG_STATS_DIR})
target_link_libraries(logStatsLib PUBLIC loggerLib)

add_executable(log_stats ${LOG_STATS_DIR}/log_stats.cpp)
target_link_libraries(log_stats PRIVATE logStatsLib)