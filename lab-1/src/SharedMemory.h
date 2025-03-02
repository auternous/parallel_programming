#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <iostream>

struct Car {
    int car_id;
    char fuel_type[10];
};

struct SharedMemory {
    Car queue[10];
    int front;
    int rear;
    int count;
};

class SharedMemoryManager {
public:
    SharedMemoryManager(key_t key, size_t size) {
        shm_id = shmget(key, size, 0666 | IPC_CREAT);
        if (shm_id == -1) {
            perror("shmget failed");
            exit(1);
        }
        shared_memory = (SharedMemory*)shmat(shm_id, nullptr, 0);
        if (shared_memory == (void*)-1) {
            perror("shmat failed");
            exit(1);
        }
        shared_memory->front = 0;
        shared_memory->rear = 0;
        shared_memory->count = 0;
    }

    ~SharedMemoryManager() {
        shmdt(shared_memory);
        shmctl(shm_id, IPC_RMID, nullptr);
    }

    SharedMemory* getSharedMemory() {
        return shared_memory;
    }

private:
    int shm_id;
    SharedMemory* shared_memory;
};

#endif // SHAREDMEMORY_H
