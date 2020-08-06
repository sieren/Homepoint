#pragma once

#include "TFT_eSPI.h"
#include <touch/TouchTypes.hpp>

#include <optional>

class TFT_eSPI;
namespace config { struct HardwareConfig; }

namespace gfx
{
  class TouchDriver
  {
    public:
      TouchDriver(TFT_eSPI* tftDriver);
      void updateHardwareConfig(config::HardwareConfig& hwConfig);
      void begin();
      void setRotation(int rotation);

      std::optional<TapEvent> tapEvent();
      
    private:
      std::optional<TouchEvent> touchPoint();
      unsigned long mLastTouchEventTime;
      TFT_eSPI* mTouch;
      TouchEvent mCurrentEvent;
      int mXAxisInversionAmount = 0;
      int mYAxisInversionAmount = 0;

  };
} // namespace gfx
