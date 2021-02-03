#pragma once

#include "ui/UIPosition.hpp"

static const unsigned long MSBeforeLongPress = 500; // 1.5s before long press
static const unsigned long MSBeforeTap = 300; // 0.2s before tap
static const unsigned long MSBeforeInvalid = 1500; // 2s before invalidate presses

static const int ScreenWidth = 320;
static const int ScreenHeight = 240;

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
