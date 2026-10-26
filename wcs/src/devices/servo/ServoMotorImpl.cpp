#include "ServoMotorImpl.h"
#include <Arduino.h>
ServoMotorImpl::ServoMotorImpl(int pin) {
  this->pin = pin;
  servo.attach(pin);
}
void ServoMotorImpl::setPercent(int percent) {
  int p = constrain(percent, 0, 100);
  int angle = map(p, 0, 100, 0, 90); // Mappa 0-100% a 0-90 gradi
  servo.write(angle);
}