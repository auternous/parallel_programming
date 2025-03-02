#ifndef PRODUCER_H
#define PRODUCER_H

#include "SharedMemory.h"
#include "Semaphore.h"
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

class Producer {
    public:
        Producer(SharedMemoryManager& shm, Semaphore& sem) : shared_memory(shm.getSharedMemory()), semaphore(sem), num_produced(0) {}
    
        void run() {
            srand(time(nullptr));
            while (num_produced < 150) { 
                //sleep(rand() % 3 + 1); 
    
                semaphore.P(1); // Блокировка очереди
    
                if (shared_memory->count < 10) { // Проверка на наличие места в очереди
                    Car new_car;
                    new_car.car_id = rand() % 1000;
                    int fuel_type = rand() % 3;
                    if (fuel_type == 0) strcpy(new_car.fuel_type, "AI-76");
                    else if (fuel_type == 1) strcpy(new_car.fuel_type, "AI-92");
                    else strcpy(new_car.fuel_type, "AI-95");
    
                    shared_memory->queue[shared_memory->rear] = new_car;
                    shared_memory->rear = (shared_memory->rear + 1) % 10;
                    shared_memory->count++;
    
                    //std::cout << "Producer: Car " << new_car.car_id << " (" << new_car.fuel_type << ") added to queue\n";
                    num_produced++; 
                }
    
                semaphore.V(1); // Разблокировка очереди
                semaphore.V(0); // Сигнализируем, что в очереди есть заявки
            }
    
            std::cout << "Producer: 150 cars generated. Stopping.\n";
        }
    
    private:
        SharedMemory* shared_memory;
        Semaphore& semaphore;
        int num_produced; // Счётчик произведённых заявок
    };

#endif // PRODUCER_H