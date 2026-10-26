#include "PotentiometerImpl.h"
#include <Arduino.h>
PotentiometerImpl::PotentiometerImpl(int pin) { this->pin = pin; }
int PotentiometerImpl::getValue() { return analogRead(pin); }