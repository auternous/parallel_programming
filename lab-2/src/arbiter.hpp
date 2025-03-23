#ifndef ARBITER_HPP
#define ARBITER_HPP

#include <iostream>
#include <vector>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>
#include "message.hpp"
#include "car.hpp"

class Arbiter {
public:
    Arbiter(int num_cars) : num_cars(num_cars) {
        msgid = msgget(MESSAGE_QUEUE_KEY, IPC_CREAT | 0666);
        if (msgid == -1) {
            perror("Arbiter: msgget failed");
            exit(1);
        }
    }

    ~Arbiter() {
        msgctl(msgid, IPC_RMID, nullptr);
    }

    void startRace() {
        std::vector<pid_t> pids;
        for (int i = 1; i <= num_cars; i++) {
            pid_t pid = fork();
            if (pid == 0) {
                Car car(i);
                car.run();
                exit(0);
            } else {
                pids.push_back(pid);
            }
        }

        std::vector<int> scores(num_cars + 1, 0);

        for (int stage = 1; stage <= 3; stage++) {
            std::cout << "Starting stage " << stage << "...\n";
            sleep(1);

            Message msg;
            msg.type = 1; // Команда старта этапа
            for (int i = 1; i <= num_cars; i++) {
                msgsnd(msgid, &msg, sizeof(Message) - sizeof(long), 0);
            }

            std::vector<std::pair<int, int>> results;
            for (int i = 0; i < num_cars; i++) {
                msgrcv(msgid, &msg, sizeof(Message) - sizeof(long), 2, 0);
                results.emplace_back(msg.time, msg.car_id);
            }

            std::sort(results.begin(), results.end());

            for (int i = 0; i < num_cars; i++) {
                scores[results[i].second] += (num_cars - i);
            }

            std::cout << "Stage " << stage << " results:\n";
            for (const auto& res : results) {
                std::cout << "Car " << res.second << " finished in " << res.first << " sec\n";
            }
        }

        std::cout << "Final Results:\n";
        for (int i = 1; i <= num_cars; i++) {
            std::cout << "Car " << i << ": " << scores[i] << " points\n";
        }

        for (auto pid : pids) {
            waitpid(pid, nullptr, 0);
        }
    }

private:
    int num_cars;
    int msgid;
};

#endif // ARBITER_HPP
