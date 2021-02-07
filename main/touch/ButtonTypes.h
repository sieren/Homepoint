#pragma once

#include <config/HID_Config.h>

namespace gfx
{
  enum class PressState: int
  {
    Start = 0,
    Running = 1,
    Ended = 2,
    None = 3
  };

  enum class Button : int
  {
    A = 0, // Left
    B = 1, // Right
    C = 2,  // Enter
    NONE = 3
  };

  struct ButtonPressState
  {
    Button btn;
    PressState state;
  };

  enum class ButtonPress : int
  {
    Tap = 0,
    LongPress = 1
  };

  struct ButtonEvent
  {
    Button button;
    ButtonPress state;
  };

} // namespace gfx
