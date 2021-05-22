#pragma once
#include <AppContext.h>
#include <config/PlatformInject.hpp>
#include <ui/UIMenuBar.h>
#include <ui/UIMosaicButton.h>
#include <ui/UIStatusBar.h>
#include <ui/UIDetailButtonBuilder.hpp>
#include <ui/UIWidgetBuilder.hpp>
#include <memory>
#include <mutex>

#include <lvgl.h>

namespace gfx
{
  class AppNewScreen : public std::enable_shared_from_this<AppNewScreen>
  {
    public:
      AppNewScreen(std::shared_ptr<ctx::AppContext> ctx);
      void loop();
      void init();
      void presentTilesForScene(const uint16_t sceneId);
      void presentScenesView();
      void showWarningScreen(const std::string warning);
      void appContextChanged(ctx::ContextState state);
      
    private:
      template<class>
      friend struct util::UISceneButtonBuilder;
      template<class>
      friend struct util::UIDetailButtonBuilder;
      lv_obj_t* createButtonContainer(lv_obj_t* pParent);
      lv_obj_t* createScreen();
      lv_obj_t* createTabOverview(lv_obj_t* pParent);
      lv_obj_t* createTab(lv_obj_t* pParent);
      std::shared_ptr<ctx::AppContext> mpAppContext;
      std::shared_ptr<UIStatusBar> mpStatusBar;
      std::shared_ptr<UIMenuBar> mpMenuBar;
      std::vector<MosaicButtonPtr> mpSceneButtons;
      std::vector<MosaicButtonPtr> mpDeviceButtons;
      lv_obj_t *mpMainMenu = NULL;
      lv_obj_t *mpSceneDetailsScreen = NULL;
      lv_obj_t *mpTilesView = NULL;
      lv_obj_t *mpButtonCont = NULL;
      std::mutex updateMutex;

  };
}