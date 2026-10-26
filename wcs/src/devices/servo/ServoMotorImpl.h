#ifndef __SERVO_MOTOR_IMPL__
#define __SERVO_MOTOR_IMPL__
#include "ServoMotor.h"
#include <Servo.h>
class ServoMotorImpl : public ServoMotor {
private: int pin; Servo servo;
public:
  ServoMotorImpl(int pin);
  void setPercent(int percent) override;
};
#endif