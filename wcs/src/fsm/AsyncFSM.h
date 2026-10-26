#ifndef __ASYNC_FSM_H__
#define __ASYNC_FSM_H__
#include "../events/Event.h"
class AsyncFSM {
public:
    virtual void handleEvent(Event* ev) = 0;
    virtual ~AsyncFSM() {}
};
#endif
