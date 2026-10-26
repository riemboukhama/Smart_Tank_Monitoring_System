#ifndef __SPECIFIC_EVENTS_H__
#define __SPECIFIC_EVENTS_H__
#include "Event.h"
#include <Arduino.h>

class ButtonEvent : public Event {
public: ButtonEvent() : Event(EV_BUTTON) {}
};

class SerialEvent : public Event {
public: 
    String payload;
    SerialEvent(String data) : Event(EV_SERIAL), payload(data) {}
};

class TimerEvent : public Event {
public: TimerEvent() : Event(EV_TIMER) {}
};
#endif
