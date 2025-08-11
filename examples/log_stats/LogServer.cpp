#include "LogServer.h"

#include <iostream>
#include <ostream>
#include <sstream>
#include <utility>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unordered_map>

#define MAX_EVENTS 10

// Конструктор для LogServer
LogServer::LogServer(std::string  ip, int port, LogStatsCollector& stats, size_t N, std::atomic<bool>& running)
    : ip_(std::move(ip)), port_(port), stats_(stats), N_(N), running_(running) {}

// Запсуск TCP севрера и чтение сокета
void LogServer::run() const {
    // Создаём сокет
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Не удалось создать сокет" << std::endl;
        return;
    }

    // Устанавливаем опцию, позволяющую переиспользовать адрес/порт сразу после закрытия сокета
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Привязка сокета к адресу и порту
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    addr.sin_addr.s_addr = inet_addr(ip_.c_str());

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        std::cerr << "Не удалось привязать сокет к адресу" << std::endl;
        close(serverSocket);
        return;
    }

    // Перевод сокета в режим прослушивания
    if (listen(serverSocket, 10) < 0) {
        std::cerr << "Не удалось начать прослушивание" << std::endl;
        close(serverSocket);
        return;
    }

    std::cout << "Сервер слушает: " << ip_ << ":" << port_ << std::endl;

    // Настройка epoll для неблокирующего ожидания событий
    int epollFD = epoll_create1(0);
    if (epollFD < 0) {
        std::cerr << "Не удалось создать epoll" << std::endl;
        close(serverSocket);
        return;
    }

    epoll_event event{};
    event.data.fd = serverSocket;
    event.events = EPOLLIN;

    // Регистрируем серверный сокет в epoll
    if (epoll_ctl(epollFD, EPOLL_CTL_ADD, serverSocket, &event) < 0) {
        std::cerr << "Не удалось добавить серверный сокет в epoll" << std::endl;
        close(serverSocket);
        close(epollFD);
        return;
    }

    epoll_event events[MAX_EVENTS]; // Массив для получения событий от epoll_wait
    std::unordered_map<int, std::string> clientBuffers; // Буферы для склейки данных

    // Основной цикл сервера
    while (running_) {
        // Ожидаем событий на сокетах. Блокируется максимум на 1 секунду
        int num_events = epoll_wait(epollFD, events, MAX_EVENTS, 1000);

        if (num_events < 0 && errno != EINTR) {
            std::cerr << "Ошибка epoll_wait()" << std::endl;
            break;
        }

        // Проходим по всем событиям, которые произошли
        for (int i = 0; i < num_events; ++i) {
            int fd = events[i].data.fd;

            // Если событие на серверном сокете - это новое соединение
            if (fd == serverSocket) {
                sockaddr_in clientAddr{};
                socklen_t clientLen = sizeof(clientAddr);
                int clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientLen);
                if (clientSocket < 0) {
                    if (!running_) break; // Если сервер завершается, ошибка ожидаема
                    std::cerr << "Не удалось принять соединение" << std::endl;
                    continue;
                }

                epoll_event clientEvent{};
                clientEvent.data.fd = clientSocket;
                clientEvent.events = EPOLLIN | EPOLLRDHUP;
                epoll_ctl(epollFD, EPOLL_CTL_ADD, clientSocket, &clientEvent);

                clientBuffers[clientSocket] = "";
                std::cout << "Клиент подключен: " << inet_ntoa(clientAddr.sin_addr) << std::endl;
            } else {
                // Данные от клиента
                char buffer[1024];
                ssize_t bytes = recv(fd, buffer, sizeof(buffer), 0);

                if (bytes <= 0 || (events[i].events & EPOLLRDHUP)) {
                    // Клиент закрыл соединение
                    close(fd);
                    clientBuffers.erase(fd);
                    epoll_ctl(epollFD, EPOLL_CTL_DEL, fd, nullptr);
                    std::cout << "Клиент отключен" << std::endl;
                }
                else {
                    buffer[bytes] = '\0';
                    clientBuffers[fd] += buffer;

                    // Разбор строк
                    size_t pos;
                    while ((pos = clientBuffers[fd].find('\n')) != std::string::npos) {
                        std::string line = clientBuffers[fd].substr(0, pos);
                        clientBuffers[fd].erase(0, pos + 1);

                        if (!line.empty()) {
                            std::cout << "Получено: " << line << std::endl;

                            std::istringstream iss(line);
                            std::string msgPart, levelPart, timePart;
                            std::getline(iss, msgPart, ',');
                            std::getline(iss, levelPart, ',');
                            std::getline(iss, timePart);

                            stats_.addLog(msgPart, levelPart);

                            if (stats_.getTotalLogs() % N_ == 0) {
                                stats_.print();
                                stats_.resetChangeFlag();
                            }
                        }
                    }
                }
            }
        }
    }

    close(epollFD);
    close(serverSocket);
}
