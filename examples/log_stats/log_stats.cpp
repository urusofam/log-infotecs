#include <atomic>
#include <csignal>
#include <iostream>
#include <string>
#include <thread>

#include "LogServer.h"
#include "LogStatsCollector.h"

std::atomic<bool> running(true);

// Обработчик SIGINT и SIGTERM
void signalHandler(int) {
    running = false;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Использование: " << argv[0] << " <ip:port> <N> <T>" << std::endl;
        return 1;
    }

    std::string target = argv[1];

    // Обрабатываем SIGINT и SIGTERM
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    // Находим :
    size_t colon_pos = target.find(':');
    if (colon_pos == std::string::npos) {
        std::cerr << "Нет : в ip:port" << std::endl;
        return 1;
    }

    // Парсим ip и порт
    std::string ip = target.substr(0, colon_pos);
    int port;
    try {
        port = std::stoi(target.substr(colon_pos + 1));
    } catch (...) {
        std::cerr << "Не удалось преобразовать порт";
        return 1;
    }

    // Парсим N и T
    int N, T;
    try {
        N = std::stoi(argv[2]);
        T = std::stoi(argv[3]);
    } catch (...) {
        std::cerr << "Не удалось преобразовать N и T";
        return 1;
    }

    // Создаём сборщик статистики логов и tcp сервер
    LogStatsCollector stats;
    LogServer server(ip, port, stats, N, running);

    // Запускаем поток сервера и таймера
    std::thread serverThread([&]() { server.run(); });
    std::thread timerThread([&]() {
        while (running) {
            std::this_thread::sleep_for(std::chrono::seconds(T));
            if (!running) break;
            if (stats.hasChangedSinceLastPrint()) {
                stats.print();
                stats.resetChangeFlag();
            }
        }
    });

    // Ждём завершения
    serverThread.join();
    timerThread.join();

    std::cout << "Программа завершена" << std::endl;
    return 0;
}
