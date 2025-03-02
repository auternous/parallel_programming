#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "SharedMemory.h"
#include "Semaphore.h"
#include "Producer.h"
#include "Consumer.h"

// Функция для чтения конфигурационного файла
std::vector<std::string> readConfig(const std::string& filename) {
    std::vector<std::string> fuel_types;
    std::ifstream config_file(filename);
    std::string line;

    if (!config_file.is_open()) {
        std::cerr << "Failed to open config file: " << filename << std::endl;
        exit(1);
    }

    while (std::getline(config_file, line)) {
        fuel_types.push_back(line);
    }

    config_file.close();
    return fuel_types;
}

int main() {
    // Создайте файл, который будет использоваться для ftok
    const char* ftok_file = "lab1.txt";
    std::ofstream file(ftok_file);
    if (!file) {
        std::cerr << "Failed to create ftok file: " << ftok_file << std::endl;
        return 1;
    }
    file.close();

    // Создание ключа для разделяемой памяти и семафоров
    key_t key = ftok(ftok_file, 65);
    if (key == -1) {
        perror("ftok failed");
        return 1;
    }

    // Инициализация разделяемой памяти
    SharedMemoryManager shm(key, sizeof(SharedMemory));

    // Инициализация семафоров
    Semaphore sem(key, 2);

    // Установка начальных значений семафоров
    sem.setValue(0, 0); // Семафор для проверки, что очередь не пуста
    sem.setValue(1, 1); // Семафор для блокировки очереди

    // Чтение конфигурационного файла
    std::vector<std::string> fuel_types = readConfig("config.txt");

    // Создание процесса Producer
    if (fork() == 0) {
        Producer producer(shm, sem);
        producer.run();
        exit(0); // Завершение дочернего процесса Producer
    }

    // Создание процессов Consumer для каждой заправки
    for (size_t i = 0; i < fuel_types.size(); i++) {
        if (fork() == 0) {
            Consumer consumer(i + 1, fuel_types[i].c_str(), shm, sem);
            consumer.run();
            exit(0); // Завершение дочернего процесса Consumer
        }
    }

    // Ожидание завершения всех дочерних процессов
    while (wait(nullptr) > 0);

    std::cout << "All processes completed. Exiting main.\n";
    return 0;
}