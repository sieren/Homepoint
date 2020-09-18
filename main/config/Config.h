#pragma once

#include <touch/ButtonDriver.h>
#include <touch/TouchDriver.h>
#include <tft/TFTESPIDriver.hpp>
#include <ui/PROGMEMIconDrawer.hpp>

#include <optional>

using ScreenDriver = gfx::driver::TFTESPI; // gfx::driver::MiniGrafxDriver;
using ImageWriter = gfx::util::PROGMEMIconDrawer<ScreenDriver>; // gfx::util::SPIFFSIconDrawer for SPIFFS only

static const unsigned long MSBeforeLongPress = 500; // 1.5s before long press
static const unsigned long MSBeforeTap = 300; // 0.2s before tap
static const unsigned long MSBeforeInvalid = 1500; // 2s before invalidate presses

static const unsigned long StatusBarUpdateInterval = 4; // update status bar every 10s to keep time up to date
static const unsigned long MinsBeforeScreenSleep = 10; // Minutes before putting Screen to sleep

static const int ScreenWidth = 320;
static const int ScreenHeight = 240;

//    _____  _            _______ ______ ____  _____  __  __ 
//   |  __ \| |        /\|__   __|  ____/ __ \|  __ \|  \/  |
//   | |__) | |       /  \  | |  | |__ | |  | | |__) | \  / |
//   |  ___/| |      / /\ \ | |  |  __|| |  | |  _  /| |\/| |
//   | |    | |____ / ____ \| |  | |   | |__| | | \ \| |  | |
//   |_|    |______/_/    \_\_|  |_|    \____/|_|  \_\_|  |_|
//                                                           
//  

#if defined(M5Stack) || defined(BUTTONS)
  // Define Button PINs
  #define BUTTON_A_PIN 39
  #define BUTTON_B_PIN 38
  #define BUTTON_C_PIN 37
  #define LED_PIN_INVERTED true // M5Stack uses Pull Up, therefore inverted.
  #define SCREEN_ROTATION_ANGLE 1
  static const bool ButtonsArePullUp = true;
  using NavigationDriver = gfx::ButtonDriver; 
  auto ScreenOnOffSwitch = [](ScreenDriver* driver, bool on, bool inverted = true)
  {
    const auto highVal = inverted ? HIGH : LOW;
    const auto lowVal = !inverted ? HIGH : LOW;
    const auto backLightPower = on ? highVal : lowVal;
    const auto screenPower = on ? ILI9341_DISPON : ILI9341_DISPOFF;
    digitalWrite(TFT_LED, backLightPower);
    driver->writeCommand(screenPower);
  };

#else // Touch Screen
  #define BUTTON_A_PIN 0 // unused
  #define BUTTON_B_PIN 0 // unused
  #define BUTTON_C_PIN 0 // unused
  #define LED_PIN_INVERTED false
  #define SCREEN_ROTATION_ANGLE 3
  using NavigationDriver = gfx::TouchDriver;
  static const bool ButtonsArePullUp = false;
  auto ScreenOnOffSwitch = [](ScreenDriver* driver, bool on, bool inverted = false)
  {
    const auto highVal = inverted ? HIGH : LOW;
    const auto lowVal = !inverted ? HIGH : LOW;
    const auto backLightPower = on ? highVal : lowVal;
    const auto screenPower = on ? ILI9341_DISPON : ILI9341_DISPOFF;
    digitalWrite(TFT_LED, backLightPower);
    driver->writeCommand(screenPower);
  };
#endif                          

namespace config
{
  struct HardwareConfig
  {
    bool mIsLEDPinInverted = LED_PIN_INVERTED;
    int mScreensaverMins = 10;
    int mScreenRotationAngle = SCREEN_ROTATION_ANGLE;
    bool mIsTouchXAxisInverted = false;
    bool mIsTouchYAxisInverted = true;
    bool mIsDisplayColorInverted = false;
  };
}