#ifndef __LCD_IMPL__
#define __LCD_IMPL__
#include "Lcd.h"
#include <LiquidCrystal_I2C.h>
class LcdImpl : public Lcd {
private: LiquidCrystal_I2C lcd;
public:
  LcdImpl(uint8_t address, uint8_t cols, uint8_t rows);
  void init() override;
  void printStatus(const char* mode, int aperture) override;
};
#endif