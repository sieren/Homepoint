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
      TouchDriver(TFT_eSPI* tftDriver, config::HardwareConfig& hwConfig);
      void begin();
      void setRotation(int rotation);

      std::optional<TapEvent> tapEvent();
      
    private:
      std::optional<TouchEvent> touchPoint();
      unsigned long mLastTouchEventTime;
      TFT_eSPI* mTouch;
      TouchEvent mCurrentEvent;
      int mXAxisInversionAmount;
      int mYAxisInversionAmount;

  };
} // namespace gfx
