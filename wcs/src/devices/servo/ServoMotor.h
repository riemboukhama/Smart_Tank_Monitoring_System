#ifndef __SERVO_MOTOR__
#define __SERVO_MOTOR__
class ServoMotor {
public:
  virtual void setPercent(int percent) = 0;
  virtual ~ServoMotor() {}
};
#endif