#pragma once

  #include "M5Touch.h"

  class M5TouchAdapter {
    public:
        // Emulates the TFT_eSPI touch interface using M5.Touch
    uint8_t getTouchRaw(uint16_t *x, uint16_t *y);
    uint16_t getTouchRawZ(void);
    void convertRawXY(uint16_t *x, uint16_t *y);
    uint8_t getTouch(uint16_t *x, uint16_t *y, uint16_t threshold = 600);
    void calibrateTouch(uint16_t *data, uint32_t color_fg, uint32_t color_bg,
                        uint8_t size);
    void setTouch(uint16_t *data);
};
// #endif /* _M5DISPLAY_H_ */
