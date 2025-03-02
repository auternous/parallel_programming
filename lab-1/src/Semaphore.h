#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <sys/ipc.h>
#include <sys/sem.h>
#include <iostream>

class Semaphore {
public:
    Semaphore(key_t key, int num_sems) {
        sem_id = semget(key, num_sems, 0666 | IPC_CREAT);
        if (sem_id == -1) {
            perror("semget failed");
            exit(1);
        }
    }

    ~Semaphore() {
        semctl(sem_id, 0, IPC_RMID);
    }

    void P(int sem_num) {
        struct sembuf op;
        op.sem_num = sem_num;
        op.sem_op = -1;
        op.sem_flg = 0;
        semop(sem_id, &op, 1);
    }

    void V(int sem_num) {
        struct sembuf op;
        op.sem_num = sem_num;
        op.sem_op = 1;
        op.sem_flg = 0;
        semop(sem_id, &op, 1);
    }

    void setValue(int sem_num, int value) {
        semctl(sem_id, sem_num, SETVAL, value);
    }

private:
    int sem_id;
};

#endif // SEMAPHORE_H