#ifndef CONSUMER_H
#define CONSUMER_H

#include "SharedMemory.h"
#include "Semaphore.h"
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

class Consumer {
public:
    Consumer(int id, const char* fuel_type, SharedMemoryManager& shm, Semaphore& sem)
        : column_id(id), fuel_type(fuel_type), shared_memory(shm.getSharedMemory()), semaphore(sem) {}

    void run() {
        while (true) {
            semaphore.P(0); // Wait for queue not empty
            semaphore.P(1); // Lock queue

            if (shared_memory->count > 0) {
                Car car = shared_memory->queue[shared_memory->front];
                if (strcmp(car.fuel_type, fuel_type) == 0) {
                    shared_memory->front = (shared_memory->front + 1) % 10;
                    shared_memory->count--;

                    std::cout << "Consumer " << column_id << ": Car " << car.car_id << " (" << car.fuel_type << ") served\n";
                    semaphore.V(1); // Unlock queue

                    sleep(rand() % 3 + 1); // Simulate service time
                    continue;
                }
            }

            semaphore.V(1); // Unlock queue
        }
    }

private:
    int column_id;
    const char* fuel_type;
    SharedMemory* shared_memory;
    Semaphore& semaphore;
};

#endif // CONSUMER_H