#include <M5Core2.h>

// Defines gestures
Gesture swipeRight("swipe right", 160, DIR_RIGHT, 30, true);
Gesture swipeDown("swipe down", 120, DIR_DOWN, 30, true);
Gesture swipeLeft("swipe left", 160, DIR_LEFT, 30, true);
Gesture swipeUp("swipe up", 120, DIR_UP, 30, true);

// Defines the buttons. Colors in format {bg, text, outline}
ButtonColors on_clrs = {RED, WHITE, WHITE};
ButtonColors off_clrs = {BLACK, WHITE, WHITE};
Button tl(0, 0, 0, 0, false ,"top-left", off_clrs, on_clrs, TL_DATUM);
Button bl(0, 0, 0, 0, false, "bottom-left", off_clrs, on_clrs, BL_DATUM);
Button tr(0, 0, 0, 0, false, "top-right", off_clrs, on_clrs, TR_DATUM);
Button br(0, 0, 0, 0, false, "bottom-right", off_clrs, on_clrs, BR_DATUM);

// For performance measurement (Single tap on bottom-right button)
uint32_t startTime;
uint32_t times = 0;

void setup() {
  M5.begin();
  M5.Buttons.addHandler(doRotation, E_GESTURE);
  M5.Buttons.addHandler(toggleColor, E_DBLTAP);
  M5.Buttons.addHandler(eventDisplay, E_ALL - E_MOVE);
  br.addHandler(showPerformance, E_TAP);
  br.repeatDelay = 1000;
  doButtons();
  startTime = millis();
}

void loop() {
  M5.update();
  times++;
}

// Positions the buttons and draws them. (Only because height and width
// change. If we were only switching between normal and upside-down we
// would only need the Buttons.draw() here.)
void doButtons() {
  int16_t hw = M5.Lcd.width() / 2;
  int16_t hh = M5.Lcd.height() / 2;
  tl.set(0, 0, hw - 5, hh - 5);
  bl.set(0, hh + 5, hw - 5, hh - 5);
  tr.set(hw + 5, 0, hw - 5, hh - 5);
  br.set(hw + 5, hh + 5, hw - 5, hh - 5);
  M5.Buttons.draw();
}

void doRotation(Event& e) {
  // Gestures and Buttons have an instanceIndex() that starts at zero
  // so by defining the gestures in the right order I can use that as
  // the input for M5.Lcd.setRotation.
  uint8_t new_rotation = e.gesture->instanceIndex();
  if (new_rotation != M5.Lcd.rotation) {
    M5.Lcd.clearDisplay();
    M5.Lcd.setRotation(new_rotation);
    doButtons();
  }
}

void toggleColor(Event& e) {
  // Just so we can type "b." instead of "e.button->"
  Button& b = *e.button;

  if (b != M5.background) {
    // Toggles the button color between black and blue
    b.off.bg = (b.off.bg == BLACK) ? BLUE : BLACK;
    b.draw();
  }
}

void showPerformance(Event& e) {
  Serial.printf("%d in %d ms, average M5.update() took %.2f microseconds\n",
   times, millis() - startTime, (float)((millis() - startTime) * 1000) / times);
  startTime = millis();
  times = 0;
}

void eventDisplay(Event& e) {
  Serial.printf("%-12s finger%d  %-18s (%3d, %3d) --> (%3d, %3d)   ",
                e.typeName(), e.finger, e.objName(), e.from.x, e.from.y,
                e.to.x, e.to.y);
  Serial.printf("( dir %d deg, dist %d, %d ms )\n", e.direction(),
                e.distance(), e.duration);
}
