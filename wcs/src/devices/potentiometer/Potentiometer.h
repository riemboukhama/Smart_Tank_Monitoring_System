#ifndef __POTENTIOMETER__
#define __POTENTIOMETER__
class Potentiometer {
public:
  virtual int getValue() = 0;
  virtual ~Potentiometer() {}
};
#endif