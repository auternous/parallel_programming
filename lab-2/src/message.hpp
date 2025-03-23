#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// Определение типа сообщения
struct Message {
    long type; // Тип сообщения (1 – команда старта, 2 – результаты)
    int car_id; // Идентификатор машины
    int time;   // Время прохождения этапа
};

#define MESSAGE_QUEUE_KEY 1234 // Ключ очереди сообщений

#endif // MESSAGE_HPP
