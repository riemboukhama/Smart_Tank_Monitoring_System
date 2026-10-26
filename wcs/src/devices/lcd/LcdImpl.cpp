#include "LcdImpl.h"
LcdImpl::LcdImpl(uint8_t address, uint8_t cols, uint8_t rows) : lcd(address, cols, rows) {}
void LcdImpl::init() {
  lcd.init();
  lcd.backlight();
}
void LcdImpl::printStatus(const char* mode, int aperture) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mode:"); lcd.print(mode);
  lcd.setCursor(0, 1);
  lcd.print("Valve:"); lcd.print(aperture); lcd.print("%");
}