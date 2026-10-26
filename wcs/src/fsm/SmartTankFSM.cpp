#include "SmartTankFSM.h"
#include <Arduino.h>

SmartTankFSM::SmartTankFSM(Potentiometer* p, ServoMotor* s, Lcd* l) {
    pot = p; servo = s; lcd = l;
    currentState = AUTO_MODE;
    valveAperture = 0;
    isConnected = true;
    lcd->init();
    servo->setPercent(valveAperture);
    updateDisplay();
}

void SmartTankFSM::updateDisplay() {
    if (!isConnected) {
        lcd->printStatus("UNCONNECTED", valveAperture);
    } else {
        lcd->printStatus(currentState == AUTO_MODE ? "AUTO" : "MANUAL", valveAperture);
    }
}

void SmartTankFSM::sendStateToSerial() {
    String mode = (currentState == AUTO_MODE) ? "AUTO" : "MANUAL";
    Serial.print("STATE:"); Serial.print(mode);
    Serial.print(",VALVE:"); Serial.println(valveAperture);
}

void SmartTankFSM::handleEvent(Event* ev) {
    switch (ev->getType()) {
        
        case EV_BUTTON: {
            currentState = (currentState == AUTO_MODE) ? MANUAL_MODE : AUTO_MODE;
            updateDisplay();
            sendStateToSerial(); 
            break;
        }
        
        case EV_SERIAL: {
            SerialEvent* sev = (SerialEvent*)ev;
            String cmd = sev->payload;
            
            if (cmd.startsWith("CMD:MODE:AUTO")) { 
                currentState = AUTO_MODE; 
                updateDisplay(); 
                sendStateToSerial(); 
            }
            else if (cmd.startsWith("CMD:MODE:MANUAL")) { 
                currentState = MANUAL_MODE; 
                updateDisplay(); 
                sendStateToSerial(); 
            }
            else if (cmd.startsWith("CMD:VALVE:")) {
                valveAperture = cmd.substring(10).toInt();
                servo->setPercent(valveAperture);
                updateDisplay();
                sendStateToSerial();
            }
            else if (cmd.startsWith("CMD:STATE:UNCONNECTED")) { isConnected = false; updateDisplay(); }
            else if (cmd.startsWith("CMD:STATE:CONNECTED")) { isConnected = true; updateDisplay(); }
            break;
        }
        
        case EV_TIMER: {
            if (currentState == MANUAL_MODE) {
                //ultima apertura per evitare variazioni del pot
                static int lastPhysicalPotAperture = 0; 
                
                int rawVal = pot->getValue();
                int newAperture = map(rawVal, 0, 1023, 0, 100);
                
                if (abs(newAperture - lastPhysicalPotAperture) > 2) { 
                    lastPhysicalPotAperture = newAperture;
                    valveAperture = newAperture;
                    servo->setPercent(valveAperture);
                    updateDisplay();
                    sendStateToSerial();
                }
            }
            break;
        }
    }
}