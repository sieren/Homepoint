#pragma once

#include <stdint.h>
extern "C"
{
  #include "lvgl.h"
}

namespace gfx
{
  struct Color
  {
    int r;
    int g;
    int b;

    uint16_t getColorInt()
    {
      return static_cast<uint16_t>((r << 16) | (g << 8) |
                                    (b << 0));
    }

    static Color BlackColor()
    {
      return {0, 0, 0};
    }

    static Color WhiteColor()
    {
      return {255, 255, 255};
    }

    static Color LightGrayColor()
    {
      return {114, 126, 145};
    }

    static Color DarkGreenColor()
    {
      return {0, 102, 102};
    }

    static Color ActiveBgColor()
    {
      return {224, 211, 96};
    }

    static Color ActiveTextColor()
    {
      return {0, 0, 0};
    }

    static Color InactiveBgColor()
    {
      return {0, 0, 0};
    }

    static Color InactiveTextColor()
    {
      return {255, 255, 255};
    }

    static Color SelectedColor()
    {
      return {185, 244, 66};
    }
  };

} // namespace gfx
