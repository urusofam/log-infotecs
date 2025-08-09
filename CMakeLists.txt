cmake_minimum_required(VERSION 3.21)
project(log_infotecs CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include_directories(include)

set(LOGGER_SOURCES
        src/FileLogger.cpp
        src/SocketLogger.cpp
        src/LoggerFactory.cpp
        src/ThreadSafeQueue.cpp
)

add_library(logger ${LOGGER_SOURCES})
target_include_directories(logger PUBLIC include)
target_link_libraries(logger PUBLIC pthread)

add_executable(log_writer examples/log_writer.cpp)
target_link_libraries(log_writer PRIVATE logger)