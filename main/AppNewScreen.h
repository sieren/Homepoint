#pragma once
#include <AppContext.h>
#include <config/PlatformInject.hpp>
#include <ui/UIStatusBar.h>
#include <memory>

#include <lvgl.h>

namespace gfx
{
  class AppNewScreen
  {
    public:
      AppNewScreen(std::shared_ptr<ctx::AppContext> ctx);
      void loop();
      void init();
      
    private:
      std::shared_ptr<ctx::AppContext> mpAppContext;
      std::shared_ptr<UIStatusBar> mpStatusBar;
      lv_obj_t *mpMainMenu; 

  };
}