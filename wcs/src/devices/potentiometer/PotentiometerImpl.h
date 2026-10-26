#ifndef __POTENTIOMETER_IMPL__
#define __POTENTIOMETER_IMPL__
#include "Potentiometer.h"
class PotentiometerImpl : public Potentiometer {
private: int pin;
public:
  PotentiometerImpl(int pin);
  int getValue() override;
};
#endif