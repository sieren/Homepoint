#pragma once

#include <optional>

static const unsigned long StatusBarUpdateInterval = 4; // update status bar every 10s to keep time up to date
static const unsigned long MinsBeforeScreenSleep = 10; // Minutes before putting Screen to sleep


//    _____  _            _______ ______ ____  _____  __  __ 
//   |  __ \| |        /\|__   __|  ____/ __ \|  __ \|  \/  |
//   | |__) | |       /  \  | |  | |__ | |  | | |__) | \  / |
//   |  ___/| |      / /\ \ | |  |  __|| |  | |  _  /| |\/| |
//   | |    | |____ / ____ \| |  | |   | |__| | | \ \| |  | |
//   |_|    |______/_/    \_\_|  |_|    \____/|_|  \_\_|  |_|
//                                                           
//  

#if defined(M5StackCore1) || defined(BUTTONS)
  // Define Button PINs
  #define BUTTON_A_PIN 39
  #define BUTTON_B_PIN 38
  #define BUTTON_C_PIN 37
  static const bool kLED_PIN_INVERTED = true; // M5Stack uses Pull Up, therefore inverted.
  static const bool kDISPLAY_INVERTED = false; // Display color space inverted
  static const bool kTOUCH_X_AXIS_INVERTED = false;
  static const bool kTOUCH_Y_AXIS_INVERTED = true;
  #define SCREEN_ROTATION_ANGLE 1
  static const bool ButtonsArePullUp = true;

#elif defined(M5StackCore2)
  // Define Button PINs
  #define BUTTON_A_PIN 0 // unused
  #define BUTTON_B_PIN 0 // unused
  #define BUTTON_C_PIN 0 // unused
  static const bool kLED_PIN_INVERTED = false; // M5Stack uses Pull Up, therefore inverted.
  static const bool kDISPLAY_INVERTED = true; // Display color space inverted
  static const bool kTOUCH_X_AXIS_INVERTED = false;
  static const bool kTOUCH_Y_AXIS_INVERTED = false;
  #define SCREEN_ROTATION_ANGLE 1
  static const bool ButtonsArePullUp = false;

#elif defined(AZTOUCHMOD)
  // Define Button PINs
  #define BUTTON_A_PIN 0 // unused
  #define BUTTON_B_PIN 0 // unused
  #define BUTTON_C_PIN 0 // unused
  static const bool kLED_PIN_INVERTED = false;
  static const bool kDISPLAY_INVERTED = false; // Display color space inverted
  static const bool kTOUCH_X_AXIS_INVERTED = true;
  static const bool kTOUCH_Y_AXIS_INVERTED = false;
  #define SCREEN_ROTATION_ANGLE 3
  static const bool ButtonsArePullUp = false;

#else // Touch Screen
  #define BUTTON_A_PIN 0 // unused
  #define BUTTON_B_PIN 0 // unused
  #define BUTTON_C_PIN 0 // unused
  static const bool kLED_PIN_INVERTED = false;
  static const bool kDISPLAY_INVERTED = false; // Display color space inverted
  static const bool kTOUCH_X_AXIS_INVERTED = false;
  static const bool kTOUCH_Y_AXIS_INVERTED = true;
  #define SCREEN_ROTATION_ANGLE 3
  static const bool ButtonsArePullUp = false;
#endif                          
