#include "EventQueue.h"
#include <Arduino.h>

EventQueue::EventQueue() {
    head = 0; tail = 0; count = 0;
}

bool EventQueue::isEmpty() {
    return count == 0;
}

void EventQueue::enqueue(Event* ev) {
    noInterrupts(); //azione atomica
    if (count < MAX_EVENTS) {
        queue[tail] = ev;
        tail = (tail + 1) % MAX_EVENTS;
        count++;
    } else {
        delete ev;
    }
    interrupts();
}

Event* EventQueue::dequeue() {
    Event* ev = nullptr;
    noInterrupts();
    if (count > 0) {
        ev = queue[head];
        head = (head + 1) % MAX_EVENTS;
        count--;
    }
    interrupts();
    return ev;
}
