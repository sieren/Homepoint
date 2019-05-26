#pragma once

#include "TFT_eSPI.h"
#include <util/optional.hpp>
#include <touch/TouchTypes.hpp>

class TFT_eSPI;

namespace gfx
{
  class TouchDriver
  {
    public:
      TouchDriver(TFT_eSPI* tftDriver);
      void begin();
      void setRotation(int rotation);

      tl::optional<TapEvent> tapEvent();
      
    private:
      tl::optional<TouchEvent> touchPoint();
      unsigned long mLastTouchEventTime;
      TFT_eSPI* mTouch;
      TouchEvent mCurrentEvent;
  };
} // namespace gfx
