#include "M5TouchAdapter.h"

// Emulates the native (resistive) TFT_eSPI touch interface using M5.Touch
 #define TOUCH		M5Touch::instance
 
uint8_t M5TouchAdapter::getTouchRaw(uint16_t *x, uint16_t *y) {
  return getTouch(x, y);
}

uint16_t M5TouchAdapter::getTouchRawZ(void) {
  return (TOUCH->ispressed()) ? 1000 : 0;
}

void M5TouchAdapter::convertRawXY(uint16_t *x, uint16_t *y) { return; }

uint8_t M5TouchAdapter::getTouch(uint16_t *x, uint16_t *y,
                            uint16_t threshold /* = 600 */) {
  TOUCH->read();
  if (TOUCH->points) {
    *x = TOUCH->point[0].x;
    *y = TOUCH->point[0].y;
    return true;
  }
  return false;
}

void M5TouchAdapter::calibrateTouch(uint16_t *data, uint32_t color_fg,
                               uint32_t color_bg, uint8_t size) {
  return;
}

void M5TouchAdapter::setTouch(uint16_t *data) { return; }