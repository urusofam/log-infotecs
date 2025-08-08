#include "SocketLogger.h"

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>

SocketLogger::SocketLogger(const std::string &serverIP, int serverPort, LogLevel logLevel) :
    socket_(-1), serverIP_(serverIP), serverPort_(serverPort), currentLogLevel_(logLevel) {}

SocketLogger::~SocketLogger() {
    if (socket_ >= 0) {
        close(socket_);
    }
}

bool SocketLogger::connectToServer() {
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ < 0) {
        std::cerr << "Не удалось создать сокет" << std::endl;
        return false;
    }

    sockaddr_in serverAddress {};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort_);

    if (inet_pton(AF_INET, serverIP_.c_str(), &serverAddress.sin_addr) <= 0) {
        std::cerr << "Не удалось конвертировать IP адрес" << std::endl;
        return false;
    }

    if (connect(socket_, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) < 0) {
        std::cerr << "Не удалось присоединиться к серверу" << std::endl;
        return false;
    }

    return true;
}

void SocketLogger::log(const std::string& message, LogLevel level) {
    if (level > currentLogLevel_) {
        return;
    }

    std::lock_guard lock(mutex_);
    if (socket_ < 0) {
        std::cerr << "Логирование в неработающий сокет" << std::endl;
        return;
    }

    std::ostringstream oss;
    oss << message << ',' << LogLevelToString(level) << ',' << getCurrentTime() << std::endl;

    std::string data = oss.str();
    ssize_t sendResult = send(socket_, data.c_str(), data.size(), 0);
    if (sendResult < 0) {
        std::cerr << "Не удалось отправить сообщение" << std::endl;
    }
}

void SocketLogger::setLogLevel(LogLevel level) {
    std::lock_guard lock(mutex_);
    currentLogLevel_ = level;
}
