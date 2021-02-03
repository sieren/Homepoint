#pragma once

#include <touch/TouchTypes.hpp>

#include <optional>

class TFT_eSPI;

namespace gfx
{
  template<class Driver = TFT_eSPI>
  class TouchDriver
  {
    public:
      using MyDriver = Driver;
      TouchDriver(Driver* tftDriver);
      void updateHardwareConfig(config::HardwareConfig& hwConfig);
      void begin();
      void setRotation(int rotation);

      std::optional<TapEvent> tapEvent();
      
    private:
      std::optional<TouchEvent> touchPoint();
      unsigned long mLastTouchEventTime;
      Driver* mTouch;
      TouchEvent mCurrentEvent;
      int mXAxisInversionAmount = 0;
      int mYAxisInversionAmount = 0;

  };
} // namespace gfx

#include "TouchDriver.ipp"
