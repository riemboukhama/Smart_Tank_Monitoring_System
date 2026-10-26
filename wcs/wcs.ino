#include "src/events/EventQueue.h"
#include "src/events/SpecificEvents.h"
#include "src/fsm/SmartTankFSM.h"
#include "src/devices/potentiometer/PotentiometerImpl.h"
#include "src/devices/servo/ServoMotorImpl.h"
#include "src/devices/lcd/LcdImpl.h"

EventQueue eventQueue;
SmartTankFSM* fsm;

const int BUTTON_PIN = 2;
unsigned long lastTimer = 0;
String serialBuffer = "";

void buttonISR() {
    static unsigned long lastInterruptTime = 0;
    unsigned long interruptTime = millis();
    //debounce
    if (interruptTime - lastInterruptTime > 200) {
        eventQueue.enqueue(new ButtonEvent()); //bloccante
    }
    lastInterruptTime = interruptTime;
}

void setup() {
    Serial.begin(115200);
    Serial.setTimeout(10);
    
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);
    
    Potentiometer* pot = new PotentiometerImpl(A0);
    ServoMotor* servo = new ServoMotorImpl(9);
    Lcd* lcd = new LcdImpl(0x27, 16, 2);
    
    fsm = new SmartTankFSM(pot, servo, lcd);
}

void loop() {
    //evento seriale
    while (Serial.available()) {
        char c = (char)Serial.read();
        if (c == '\n') {
            eventQueue.enqueue(new SerialEvent(serialBuffer));
            serialBuffer = "";
        } else {
            serialBuffer += c;
        }
    }
    
    //eventi temporali
    if (millis() - lastTimer >= 50) {
        lastTimer = millis();
        eventQueue.enqueue(new TimerEvent());
    }
    
    //estrazione dalla coda
    if (!eventQueue.isEmpty()) {
        Event* ev = eventQueue.dequeue();
        fsm->handleEvent(ev);
        delete ev;
    }
}
