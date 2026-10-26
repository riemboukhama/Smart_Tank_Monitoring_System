#ifndef __EVENT_H__
#define __EVENT_H__
enum EventType { EV_BUTTON, EV_SERIAL, EV_TIMER };
class Event {
protected:
    EventType type;
public:
    Event(EventType t) : type(t) {}
    EventType getType() { return type; }
    virtual ~Event() {}
};
#endif
