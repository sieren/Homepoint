#pragma once

#include <touch/ButtonDriver.h>
#include <touch/TouchDriver.h>
#include <tft/TFTESPIDriver.hpp>
#include <ui/PROGMEMIconDrawer.hpp>

using ScreenDriver = gfx::driver::TFTESPI; // gfx::driver::MiniGrafxDriver;
using ImageWriter = gfx::util::PROGMEMIconDrawer<ScreenDriver>; // gfx::util::SPIFFSIconDrawer for SPIFFS only

static const unsigned long MSBeforeLongPress = 500; // 1.5s before long press
static const unsigned long MSBeforeTap = 300; // 0.2s before tap
static const unsigned long MSBeforeInvalid = 1500; // 2s before invalidate presses

static const unsigned long StatusBarUpdateInterval = 10; // update status bar every 10s to keep time up to date
static const unsigned long MinsBeforeScreenSleep = 10; // Minutes before putting Screen to sleep

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
  static const int Rotation = 1; // Set rotation angle
  static const bool ButtonsArePullUp = true;
  using NavigationDriver = gfx::ButtonDriver; 
  // M5Stack uses Pull Up, therefore inverted.
  auto ScreenOnOffSwitch = [](ScreenDriver* driver, bool on)
  {
    const auto backLightPower = on ? HIGH : LOW;
    const auto screenPower = on ? ILI9341_DISPON : ILI9341_DISPOFF;
    digitalWrite(TFT_LED, backLightPower);
    driver->writeCommand(screenPower);
  };

// Please define PINs in libraries/TFT_ESPI/User_Setup.h
// #define M5Stack
// #define TFT_MISO 19
// #define TFT_MOSI 23
// #define TFT_SCLK 18
// #define TFT_CS   14  // Chip select control pin
// #define TFT_DC   27  // Data Command control pin
// #define TFT_RST  33  // Reset pin (could connect to Arduino RESET pin)
// #define TFT_BL   32  // LED back-light (required for M5Stack)
// #define TFT_LED   32

#else // Touch Screen
  #define BUTTON_A_PIN 0 // unused
  #define BUTTON_B_PIN 0 // unused
  #define BUTTON_C_PIN 0 // unused
  static const int Rotation = 3; // Set rotation angle
  using NavigationDriver = gfx::TouchDriver;
  static const bool ButtonsArePullUp = false;
  auto ScreenOnOffSwitch = [](ScreenDriver* driver, bool on)
  {
    const auto state = on ? LOW : HIGH;
    const auto screenPower = on ? ILI9341_DISPON : ILI9341_DISPOFF;
    digitalWrite(TFT_LED, state);
    driver->writeCommand(screenPower);
  };
#endif                          

// Please define PINs in libraries/TFT_ESPI/User_Setup.h
// The following are examples for ArduiTouch (ILI9341 Display)
// #define TFT_CS   5
// #define TFT_DC   4
// #define TFT_MOSI 23
// #define TFT_CLK  18
// #define TFT_RST  22
// #define TFT_MISO 19
// #define TFT_LED  15  

// #define HAVE_TOUCHPAD
// #define TOUCH_CS 14
// #define TOUCH_IRQ 2
/*_______End of definitions______*/

// M5Stack Definitions
