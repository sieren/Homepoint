#include <M5Core2.h>

void setup() {
  M5.begin();
  M5.Lcd.fillScreen(WHITE);
}

void loop() {
  M5.update();
  Event& e = M5.Buttons.event;
  if (e & (E_MOVE | E_RELEASE)) circle(e & E_MOVE ? e.from : e.to, WHITE);
  if (e & (E_TOUCH | E_MOVE)) circle(e.to, e.finger ? BLUE : RED);
}

void circle(Point p, uint16_t c) {
  M5.Lcd.drawCircle(p.x, p.y, 50, c);
  M5.Lcd.drawCircle(p.x, p.y, 52, c);
}
