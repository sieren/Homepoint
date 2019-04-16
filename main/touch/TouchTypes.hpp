#pragma once

#include "ui/UIPosition.hpp"
namespace gfx
{

  enum class TouchState: int
  {
    TouchStart = 0,
    TouchRunning = 1,
    TouchEnded = 2,
    NoTouch = 3
  };

  struct TouchEvent
  {
    Point position;
    TouchState state;
    unsigned long timestamp;
  };

  enum class PressEvent : int
  {
    Tap = 0,
    LongPress = 1,
    None = 9
  };

  struct TapEvent
  {
    PressEvent state;
    Point position;
    unsigned long timestamp;
  };

} // namespace gfx