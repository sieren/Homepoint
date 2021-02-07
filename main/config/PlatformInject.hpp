#pragma once

#include <touch/ButtonDriver.h>
#include <touch/TouchDriver.h>
#include <ui/PROGMEMIconDrawer.hpp>

//    _____  _            _______ ______ ____  _____  __  __ 
//   |  __ \| |        /\|__   __|  ____/ __ \|  __ \|  \/  |
//   | |__) | |       /  \  | |  | |__ | |  | | |__) | \  / |
//   |  ___/| |      / /\ \ | |  |  __|| |  | |  _  /| |\/| |
//   | |    | |____ / ____ \| |  | |   | |__| | | \ \| |  | |
//   |_|    |______/_/    \_\_|  |_|    \____/|_|  \_\_|  |_|
//                                                           
//  

#if defined(M5StackCore1) || defined(BUTTONSASS)
  #include <tft/TFTESPIDriver.hpp>
  using NavigationDriver = gfx::ButtonDriver; 
  using ScreenDriver = gfx::driver::TFTESPI; // gfx::driver::MiniGrafxDriver;
  auto ScreenOnOffSwitch = [](ScreenDriver* driver, bool on, bool inverted = true)
  {
    const auto highVal = inverted ? HIGH : LOW;
    const auto lowVal = !inverted ? HIGH : LOW;
    const auto backLightPower = on ? highVal : lowVal;
    const auto screenPower = on ? ILI9341_DISPON : ILI9341_DISPOFF;
    digitalWrite(TFT_LED, backLightPower);
    driver->writeCommand(screenPower);
  }; // Screensaver calls to disable screen
  auto InitializeScreen = [](ScreenDriver* driver)
  {
    driver->begin(320000000);
  }; // Screen initialization routines
  auto Updater = []() {}; // misc. hardware update operations. called from loop()

#elif defined(M5StackCore2)
  #include <tft/TFTM5StackDriver.hpp>
  #include <M5TouchAdapter.h>
  using NavigationDriver = gfx::TouchDriver<M5TouchAdapter>;
  using ScreenDriver = gfx::driver::TFTM5STACKESPI; 
  auto ScreenOnOffSwitch = [](ScreenDriver* driver, bool on, bool inverted = false)
  {
    driver->setSleep(!on);
  };
  auto InitializeScreen = [](ScreenDriver* driver)
  {
    M5.begin(true, false, false,/* I2C */ false);
    M5.Axp.SetLDOEnable(3,0); // disable vibration motor
    driver->begin();
  };
  auto Updater = []()
  {
  };
  
#else // Touch Screen
  #include <tft/TFTESPIDriver.hpp>
  using NavigationDriver = gfx::TouchDriver<TFT_eSPI>;
  using ScreenDriver = gfx::driver::TFTESPI; // gfx::driver::MiniGrafxDriver;
  auto ScreenOnOffSwitch = [](ScreenDriver* driver, bool on, bool inverted = false)
  {
    const auto highVal = inverted ? HIGH : LOW;
    const auto lowVal = !inverted ? HIGH : LOW;
    const auto backLightPower = on ? highVal : lowVal;
    const auto screenPower = on ? ILI9341_DISPON : ILI9341_DISPOFF;
    digitalWrite(TFT_LED, backLightPower);
    driver->writeCommand(screenPower);
  };
  auto InitializeScreen = [](ScreenDriver* driver)
  {
    driver->begin(320000000);
  }; // Screen initialization routines
  auto Updater = []() {};
#endif                          

using ImageWriter = gfx::util::PROGMEMIconDrawer<ScreenDriver>; // gfx::util::SPIFFSIconDrawer for SPIFFS only
