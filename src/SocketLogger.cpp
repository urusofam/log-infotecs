#include "SocketLogger.h"

#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>

// Конструктор для сокет логера
SocketLogger::SocketLogger(const std::string &serverIP, int serverPort, LogLevel logLevel) :
    socket_(-1), serverIP_(serverIP), serverPort_(serverPort), currentLogLevel_(logLevel) {}

// Деструктор для сокет логера
SocketLogger::~SocketLogger() {
    if (socket_ >= 0) {
        close(socket_);
    }
}

// Функция соеденинения с сервером через сокет
bool SocketLogger::connectToServer() {
    // Создание сокета
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ < 0) {
        std::cerr << "Не удалось создать сокет" << std::endl;
        return false;
    }

    // Настройка адреса сервера
    sockaddr_in serverAddress {};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort_);

    // Преобразование IP адреса
    if (inet_pton(AF_INET, serverIP_.c_str(), &serverAddress.sin_addr) <= 0) {
        std::cerr << "Не удалось конвертировать IP адрес" << std::endl;
        return false;
    }

    // Подключение
    if (connect(socket_, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) < 0) {
        std::cerr << "Не удалось присоединиться к серверу" << std::endl;
        return false;
    }

    return true;
}

// Функция логирования для сокет логера
void SocketLogger::log(const std::string& message, LogLevel level) {
    // Если уровень переданного сообщения ниже текущего
    if (level > currentLogLevel_) {
        return;
    }

    std::lock_guard lock(mutex_);
    // Если невалидный сокет
    if (socket_ < 0) {
        std::cerr << "Логирование в неработающий сокет" << std::endl;
        return;
    }

    // Готовим строку к отправке
    std::ostringstream oss;
    oss << message << ',' << LogLevelToString(level) << ',' << getCurrentTime() << std::endl;
    std::string data = oss.str();

    // Отправляем
    ssize_t sendResult = send(socket_, data.c_str(), data.size(), 0);
    if (sendResult < 0) {
        std::cerr << "Не удалось отправить сообщение" << std::endl;
    }
}

// Функция установки уровня логирования
void SocketLogger::setLogLevel(LogLevel level) {
    std::lock_guard lock(mutex_);
    currentLogLevel_ = level;
}
