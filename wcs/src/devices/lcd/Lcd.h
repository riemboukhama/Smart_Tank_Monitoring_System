#ifndef __LCD__
#define __LCD__
class Lcd {
public:
  virtual void init() = 0;
  virtual void printStatus(const char* mode, int aperture) = 0;
  virtual ~Lcd() {}
};
#endif