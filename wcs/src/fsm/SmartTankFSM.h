#ifndef __SMART_TANK_FSM_H__
#define __SMART_TANK_FSM_H__
#include "AsyncFSM.h"
#include "../events/SpecificEvents.h"
#include "../devices/potentiometer/Potentiometer.h"
#include "../devices/servo/ServoMotor.h"
#include "../devices/lcd/Lcd.h"

class SmartTankFSM : public AsyncFSM {
public:
    enum State { AUTO_MODE, MANUAL_MODE };
private:
    State currentState;
    Potentiometer* pot;
    ServoMotor* servo;
    Lcd* lcd;
    int valveAperture;
    bool isConnected;

    void updateDisplay();
    void sendStateToSerial();
public:
    SmartTankFSM(Potentiometer* p, ServoMotor* s, Lcd* l);
    void handleEvent(Event* ev) override;
};
#endif
