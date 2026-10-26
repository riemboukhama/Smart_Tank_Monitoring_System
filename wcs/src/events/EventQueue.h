#ifndef __EVENT_QUEUE_H__
#define __EVENT_QUEUE_H__
#include "Event.h"
#define MAX_EVENTS 15

class EventQueue {
private:
    Event* queue[MAX_EVENTS];
    volatile int head;
    volatile int tail;
    volatile int count;
public:
    EventQueue();
    bool isEmpty();
    void enqueue(Event* ev);
    Event* dequeue();
};
#endif
