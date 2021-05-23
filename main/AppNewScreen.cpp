#include "AppNewScreen.h"
#include <ui/Styles.h>
#include <config/PlatformInject.hpp>
#include <util/varianthelper.hpp>
/// Additional LVGL Custom Drivers
#define LV_PNG_USE_LV_FILESYSTEM 1

extern "C"
{
  #include "lvgl.h"
  #include "lvgl_helpers.h"
  #include <fs/spiffs_drv.h>
  #include "axp192.h"
}

#define LV_TICK_PERIOD_MS 1
#include <memory>

static void gui_timer_tick(void *arg)
{
	// Unused
	(void) arg;

	lv_tick_inc(LV_TICK_PERIOD_MS);
}

namespace gfx 
{
  AppNewScreen::AppNewScreen(std::shared_ptr<ctx::AppContext> ctx) :
    mpAppContext(ctx)
  {
    
  }

  void AppNewScreen::init()
  {
    static lv_color_t bufs[2][DISP_BUF_SIZE];
    static lv_disp_buf_t disp_buf;
    uint32_t size_in_px = DISP_BUF_SIZE;

    // Set up the frame buffers
    lv_disp_buf_init(&disp_buf, &bufs[0], &bufs[1], size_in_px);

    // Set up filesystem
    spiffs_drv_init();
  
    // Set up the display driver
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = disp_driver_flush;
    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);

    disp_drv.flush_cb = disp_driver_flush;
    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);

    lv_indev_drv_t indev_drv;
	  lv_indev_drv_init(&indev_drv);
	  indev_drv.read_cb = touch_driver_read;
	  indev_drv.type = LV_INDEV_TYPE_POINTER;
	  lv_indev_drv_register(&indev_drv);
  
    const esp_timer_create_args_t periodic_timer_args = {
      .callback = &gui_timer_tick,
      .name = "periodic_gui"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));

    using namespace std::placeholders;
    mpAppContext->registerStateCallback(std::bind(&AppNewScreen::appContextChanged, this, _1));
    Serial.println("Done setting up the GUI");
  }

  lv_obj_t* AppNewScreen::createScreen()
  {
    lv_obj_t *screen = lv_cont_create(NULL, NULL); 
    lv_cont_set_layout(screen, LV_LAYOUT_COLUMN_MID);
    lv_obj_set_style_local_bg_color (screen, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
	  lv_obj_set_style_local_bg_opa( screen, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER); 
    lv_obj_add_style(screen, LV_OBJ_PART_MAIN, &Styles::getInstance().mainStyle);
    if (!mpStatusBar)
    {
      mpStatusBar = std::make_shared<UIStatusBar>(
        UIStatusBar(screen, mpAppContext->getWifiContext(), mpAppContext->getMQTTConnection()));
      mpStatusBar->setupCallbacks();
    }
    else
    {
      mpStatusBar->setupScreen(screen);
    }
    return screen;
  }

  void AppNewScreen::presentScenesView()
  {
    if (mpMainMenu) { lv_obj_del(mpMainMenu); }
    mpMainMenu = createScreen();
    auto tabOverview = createTabOverview(mpMainMenu);
    auto tabView = createTab(tabOverview);
    auto buttonCont = createButtonContainer(tabView);
    mpSceneButtons.clear();
    auto& scenes = mpAppContext->getMQTTGroups();
    int counter = 0;
    for (auto& scene : scenes)
    {
      std::visit(::util::overloaded([&](auto&& ptr)
      {
        if (counter%6 == 0 && counter != 0)
        {
          tabView = createTab(tabOverview);
          buttonCont = createButtonContainer(tabView);
        }
        counter++;
        auto button = util::UISceneButtonBuilder<decltype(this)>()(ptr, this, buttonCont);
        mpSceneButtons.push_back(button);
      }), scene);
    }
    lv_scr_load(mpMainMenu);
  }

  void AppNewScreen::presentTilesForScene(const uint16_t sceneId)
  {
    if (mpSceneDetailsScreen) { lv_obj_del(mpSceneDetailsScreen); }
    mpSceneButtons.clear();
    mpSceneDetailsScreen = createScreen();
    mpStatusBar->setAction({MenuActionState::MAINSCREEN, [&]() {
      presentScenesView();
    }});
    auto& scenes = mpAppContext->getMQTTGroups();
    mpDeviceButtons = std::vector<MosaicButtonPtr>();

    auto mqttScene = std::find_if(scenes.begin(), scenes.end(), [&](auto& scene)
    {
      return std::visit(::util::overloaded([&](auto&& ptr) {
        return ptr->groupId == sceneId;
      }), scene);
    });

    int counter = 0;
    auto tabView = createTabOverview(mpSceneDetailsScreen);
    auto tab = createTab(tabView);
    auto buttonCont = createButtonContainer(tab);
    std::visit(::util::overloaded([&](MQTTSwitchGroupPtr ptr) {
      for (auto &switchDevice : ptr->mDevices)
      {
        if (counter%6 == 0 && counter != 0)
        {
          tab = createTab(tabView);
          buttonCont = createButtonContainer(tab);
        }
        counter++;
        std::shared_ptr<UIMosaicButton> button;
        button = util::UIDetailButtonBuilder<decltype(this)>()(switchDevice, ptr, this, buttonCont);
        mpDeviceButtons.push_back(button);
      }
    },
    [](auto&& ptr){}), *mqttScene);

    if (counter%6 == 0 && counter != 0)
    {
      tab = createTab(tabView);
      buttonCont = createButtonContainer(tab);
    }

    auto backButton = std::make_shared<UIMosaicButton>(buttonCont, "Back");
    backButton->setupButton();
    backButton->setImage("exitbutton");
    backButton->addTargetAction([&](const uint16_t id) {
      presentScenesView();
    });
    mpDeviceButtons.push_back(backButton);
    using namespace std::placeholders;
    const std::string sceneName = std::visit(::util::overloaded([&](auto&& ptr)
      {
        return ptr->sceneName;
      }), *mqttScene);
    lv_scr_load(mpSceneDetailsScreen);
  }

  lv_obj_t* AppNewScreen::createTabOverview(lv_obj_t* pParent)
  {
      lv_obj_t* tabView = lv_tabview_create(pParent, NULL);
      lv_obj_add_style(tabView, LV_TABVIEW_PART_BG, &Styles::getInstance().mainStyle);
      lv_tabview_set_btns_pos(tabView, LV_TABVIEW_TAB_POS_NONE);
      return tabView;
  }

  lv_obj_t* AppNewScreen::createTab(lv_obj_t* pParent)
  {
    auto tab = lv_tabview_add_tab(pParent, "xx");
    lv_obj_set_style_local_pad_left(tab, LV_TABVIEW_PART_BG_SCROLLABLE , LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_pad_right(tab, LV_TABVIEW_PART_BG_SCROLLABLE , LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_pad_top(tab, LV_TABVIEW_PART_BG_SCROLLABLE , LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_pad_bottom(tab, LV_TABVIEW_PART_BG_SCROLLABLE , LV_STATE_DEFAULT, 0);
    lv_obj_add_style(tab, LV_PAGE_PART_SCROLLABLE, &Styles::getInstance().mainStyle);
    return tab;
  }

  lv_obj_t* AppNewScreen::createButtonContainer(lv_obj_t* pParent)
  {
    lv_obj_t* pButtonCont = lv_cont_create(pParent, NULL);
    lv_cont_set_layout(pButtonCont, LV_LAYOUT_PRETTY_MID);
    lv_obj_set_size(pButtonCont, lv_obj_get_width(pParent), lv_obj_get_height(pParent));
    lv_obj_set_auto_realign(pButtonCont, true);     
    lv_cont_set_fit2(pButtonCont, LV_FIT_MAX, LV_FIT_NONE);
    lv_obj_add_style(pButtonCont, LV_OBJ_PART_MAIN, &Styles::getInstance().mainStyle);
    lv_page_glue_obj(pButtonCont, true);
    lv_obj_set_style_local_pad_top(pButtonCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
    return pButtonCont;
  }

  void AppNewScreen::showWarningScreen(const std::string warning)
  {
    auto warningScreen = createScreen();
    auto tabOverview = createTabOverview(warningScreen);
    auto tabView = createTab(tabOverview);
    auto buttonCont = createButtonContainer(tabView);
    lv_obj_t* warningLabel = lv_label_create(buttonCont, NULL);
    lv_label_set_long_mode(warningLabel, LV_LABEL_LONG_BREAK);
    lv_label_set_align(warningLabel, LV_LABEL_ALIGN_CENTER);
    lv_obj_set_width(warningLabel, 250);
    const std::string warningIcon{LV_SYMBOL_WARNING};
    auto displayString = warningIcon + "\n" + warning;
    lv_label_set_text(warningLabel, displayString.c_str());
    lv_scr_load(warningScreen);
  }

  void AppNewScreen::appContextChanged(ctx::ContextState state)
  {
    using namespace ctx;
    switch (state) {
      case ContextState::Reload:
      {
        // reboot stub
        break;
      }
      case ContextState::Ready:
      {
        std::lock_guard<std::mutex> guard(updateMutex);
        using namespace std::placeholders;
        break;
      }
    }
  }
  void AppNewScreen::loop()
  {
    std::lock_guard<std::mutex> guard(updateMutex);
    lv_task_handler(); // process animations
  }

} // namespace gfx