#pragma once

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
    pinMode(TFT_LED, OUTPUT);
    digitalWrite(TFT_LED, LOW);
  //  driver->begin(320000000);
  }; // Screen initialization routines
  auto InitializePlatform = []() {};

#elif defined(M5StackCore2)
  #include <PlatformM5StackCore2.hpp>
  
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
    pinMode(TFT_LED, OUTPUT);
    digitalWrite(TFT_LED, LOW);    // LOW to turn backlight on - pcb version 01-02-00
  //  driver->begin(320000000);
  }; // Screen initialization routines
  auto InitializePlatform = []() {};
#endif                          
