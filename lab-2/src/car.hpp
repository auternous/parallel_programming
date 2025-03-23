#ifndef CAR_HPP
#define CAR_HPP

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sys/msg.h>
#include <unistd.h>
#include "message.hpp"

class Car {
public:
    explicit Car(int id) : car_id(id) {}

    void run() {
        int msgid = msgget(MESSAGE_QUEUE_KEY, 0666);
        if (msgid == -1) {
            perror("Car: msgget failed");
            exit(1);
        }

        for (int stage = 1; stage <= 3; stage++) {
            Message msg;
            msgrcv(msgid, &msg, sizeof(Message) - sizeof(long), 1, 0); // Ждём старт этапа

            int time = rand() % 5 + 1; // Время этапа от 1 до 5 секунд
            sleep(time);

            msg.type = 2; // Сообщение арбитру о завершении
            msg.car_id = car_id;
            msg.time = time;

            msgsnd(msgid, &msg, sizeof(Message) - sizeof(long), 0);
            std::cout << "Car " << car_id << " finished stage " << stage << " in " << time << " sec\n";
        }
    }

private:
    int car_id;
};

#endif // CAR_HPP
