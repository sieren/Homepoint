#pragma once
#include "lvgl.h"

namespace gfx
{
  class Styles
  { 
    public:
      static Styles &getInstance()
      {
        static Styles instance;
        return instance;
      };
      static void init();
      Styles();
      lv_style_t mainStyle;
      lv_style_t switchBtnContStyle;
      lv_style_t sensorBtnContStyle;

    private:
  };
} // namespace gfx
