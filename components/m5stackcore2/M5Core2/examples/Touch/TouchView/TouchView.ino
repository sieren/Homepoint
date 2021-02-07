#include <M5Core2.h>

// Display all the touch event on the M5Core2 screen.
// Press the A button to turn on/off E_MOVE event detection
// Press the B button to turn on/off long press (500mS) detection
// Press the C button to turn on/off key repeat (200mS)


TFT_eSprite disp(&M5.Lcd);
bool first_scroll = true;
bool show_move    = false;
bool long_press   = true;
bool key_repeat   = false;

// Defines gestures
Gesture swipeRight("Swipe Right", 80, DIR_RIGHT, 30, true);
Gesture swipeDown( "Swipe Down",  60, DIR_DOWN,  30, true);
Gesture swipeLeft( "Swipe Left",  80, DIR_LEFT,  30, true);
Gesture swipeUp(   "Swipe Up",    60, DIR_UP,    30, true);


// Use a scrollable sprite for output. Looks nice!
//
void setup_disp() {
  disp.createSprite(320, 240);
  disp.setScrollRect(0, 0, 320, 240);
  disp.fillSprite(BLACK);
  disp.setTextFont(2);
  disp.setTextSize(1);
  disp.setTextColor(WHITE, BLACK);
  disp.setCursor(0, 0);
  disp.pushSprite(0, 0);
}


// Lazy output routine. Just enough to do the job.
// str should fit on the screen (320 pixels) and not contain a \n
//
void output_info(const char* name, const char* info) {
  Serial.printf("%-15s: %s\n", name, info);
  if(disp.getCursorY() >= 220) {
    disp.scroll(0, first_scroll ? -3 : -16);
    first_scroll = false;
  }
  disp.printf("%s", name);
  disp.setCursor(110, disp.getCursorY());
  disp.printf("%s\n", info);
  if(disp.getCursorY() >= 220) {
    disp.setCursor(0, 220);
  }
  disp.pushSprite(0, 0);
}


// Let the user know what to do
//
void splash_screen() {
  output_info("",               "Welcome to TouchView");
  output_info("Button A Sets", (show_move)  ? "E_MOVE will be displayed"      : "E_MOVE will be ignored");
  output_info("Button B Sets", (long_press) ? "Long Presses will be detected" : "Long Presses will be ignored");
  output_info("Button C Sets", (key_repeat) ? "Key Repeat Enabled"            : "Key Repeat Disabled");
  output_info("To Use",        "Touch the Screen");
}


void eventHandler(Event& e) {
  char buffer[32];
  sprintf(buffer, "%3d,%3d/%3d,%3d %3d %3d %3d", e.from.x, e.from.y, e.to.x, e.to.y, e.distance(), e.direction(), e.duration);
  output_info(e.typeName(), buffer);
}

void gestureHandler(Event& e) {
  output_info(e.typeName(), e.gesture->getName());
}


// Remove/replace event handlers, since list of evetns could change.
//
void setup_events() {
  M5.background.delHandlers();
  uint16_t events              = (show_move)  ? E_ALL : (E_ALL - E_MOVE); // Show all events, or everything but E_MOVE? Controlled with A button.
  M5.background.longPressTime  = (long_press) ? 500   : 0;                // Detect long presses (500mS) or not? Controlled with B button.
  M5.background.repeatDelay    = (key_repeat) ? 200   : 0;                // Repeat press events every 200mS or not? Controlled with the C button.
  M5.background.addHandler(eventHandler, events);
}


// Gestues only need to be set up once. I'll protect against multiple initializations.
//
void setupGestures() {
  static bool done = false;
  if(!done) {
    done = true;
    swipeRight.addHandler(gestureHandler, E_GESTURE);
    swipeLeft.addHandler(gestureHandler, E_GESTURE);
    swipeUp.addHandler(gestureHandler, E_GESTURE);
    swipeDown.addHandler(gestureHandler, E_GESTURE);
  }
}


void setup() {
  M5.begin();
  setup_disp();
  splash_screen();
  setup_events();
  setupGestures();
}


void loop() {
  M5.update();
  if(M5.BtnA.wasPressed()) {
    show_move = !show_move;
    output_info("Button A", (show_move) ? "E_MOVE will be displayed" : "E_MOVE will be ignored");
    setup_events();
  }
  
  if(M5.BtnB.wasPressed()) {
    long_press = !long_press;
    output_info("Button B", (long_press) ? "Long Presses will be detected" : "Long Presses will be ignored");
    setup_events();
  }
  
  if(M5.BtnC.wasPressed()) {
    key_repeat = !key_repeat;
    output_info("Button C", (key_repeat) ? "Key Repeat Enabled" : "Key Repeat Disabled");
    setup_events();
  }
}
