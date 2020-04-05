#include <AppScreen.hpp>
#include <config/Icon.hpp>
#include <ui/UIDetailButtonBuilder.hpp>
#include <ui/ScreenNavigator.hpp>
#include <ui/UIMosaicMenuWidget.hpp>
#include <ui/UIWidgetBuilder.hpp>
#include <ui/UIErrorWidget.hpp>
#include <touch/TouchDriver.h>
#include <util/varianthelper.hpp>

#include "Arduino.h"
#include "SPI.h"

#include <thread>

extern "C" 
{
  #include "esp_log.h"
}

#include <iostream>

namespace gfx
{
  static const int kStatusBarHeight = 20;

  template<class ScreenDriver, class NavigationDriver>
  AppScreen<ScreenDriver, NavigationDriver>::AppScreen(std::shared_ptr<ctx::AppContext> ctx, Size size) :
    UIWidget(nullptr, Frame{{0,0,0}, size}, 10000),
    mWindowSize(size),
    mViewPortSize({mWindowSize.width, mWindowSize.height - kStatusBarHeight}),
    menuFrame({{0, kStatusBarHeight, 0}, mViewPortSize}),
    mTft(size.width, size.height),
    mNavigation(mTft.getDriverRef()),
    mpAppContext(ctx),
    mpStatusBar(new UIStatusBarWidget(&mTft, Frame{{0,0,0}, {size.width, kStatusBarHeight}}, 999)),
    mScreenSaver(&mTft, ctx)
  {
  };


  template<class ScreenDriver, class NavigationDriver>
  void AppScreen<ScreenDriver, NavigationDriver>::presentScreen(const uint16_t sceneId)
  {
    auto& scenes = mpAppContext->getMQTTGroups();
    auto widgets = std::vector<WidgetPtr>();

    auto mqttScene = std::find_if(scenes.begin(), scenes.end(), [&](auto& scene)
    {
      return std::visit(::util::overloaded([&](auto&& ptr) {
        return ptr->groupId == sceneId;
      }), scene);
    });

    widgets = std::visit(::util::overloaded([&](auto&& ptr)
    {
      return util::UIDetailButtonBuilder<decltype(this)>()(ptr, this);
    }), *mqttScene);
    const auto menuFrame = Frame{{0, kStatusBarHeight, 0}, mViewPortSize};
  
    auto screenNavigator = std::make_shared<ScreenNavigator<NavigationDriver>>(&mTft, menuFrame, 1000);
    screenNavigator->presentDismissalSubviews(widgets, [&](const uint16_t tagId)
    {
      dismissPresentingScreen(tagId);
    });
    using namespace std::placeholders;
    mIsPresenting = true;
    const std::string sceneName = std::visit(::util::overloaded([&](auto&& ptr)
      {
        return ptr->sceneName;
      }), *mqttScene);

    // Spawn a thread and wait for the draw-cycle to finish before
    // we add/remove new views.
    std::thread addViewThread([&, sceneName, screenNavigator]()
    {
      std::lock_guard<std::mutex> lock(viewMutex);
      mpSubViews.pop_back();
      mpSubViews.push_back(screenNavigator);
      mpStatusBar->setTextLabel(sceneName);
    });
    addViewThread.detach();

  }

  template<class ScreenDriver, class NavigationDriver>
  void AppScreen<ScreenDriver, NavigationDriver>::setupScreen()
  {
    mTft.begin(320000000);
    mTft.setRotation(mpAppContext->getModel().mHardwareConfig.mScreenRotationAngle);
    mpStatusBar->setBackgroundColor(Color::BlackColor());
    mpStatusBar->setTextColor(Color::WhiteColor());

    ledcSetup(0, 2000, 8);
    ledcAttachPin(SDA, 0);
  }

  template<class ScreenDriver, class NavigationDriver>
  void AppScreen<ScreenDriver, NavigationDriver>::appContextChanged(ctx::ContextState state)
  {
    using namespace ctx;
    switch (state) {
      case ContextState::Reload:
      {
        std::lock_guard<std::mutex> guard(viewMutex);
        mpSubViews.clear();
        break;
      }
      case ContextState::Ready:
      {
        std::lock_guard<std::mutex> guard(viewMutex);
        using namespace std::placeholders;
        mpAppContext->getMQTTConnection()->registerConnectionStatusCallback(
          std::bind(&UIStatusBarWidget::mqttConnectionChanged, mpStatusBar.get(), _1));
        presentMenu();
        break;
      }
    }
  }

  template<class ScreenDriver, class NavigationDriver>
  void AppScreen<ScreenDriver, NavigationDriver>::registerWifiCallback()
  {
    mpStatusBar->registerCallback(&mpAppContext->getWifiContext());
  }

  template<class ScreenDriver, class NavigationDriver>
  void AppScreen<ScreenDriver, NavigationDriver>::setupData()
  {
    using namespace std::placeholders;
    registerWifiCallback();
    mpAppContext->registerStateCallback(std::bind(&AppScreen::appContextChanged, this, _1));
    mpAppContext->getMQTTConnection()->registerConnectionStatusCallback(std::bind(&UIStatusBarWidget::mqttConnectionChanged, mpStatusBar.get(), _1));

    mpSubViews.clear();
    presentMenu();
  }

  template<class ScreenDriver, class NavigationDriver>
  void AppScreen<ScreenDriver, NavigationDriver>::presentMenu()
  {
    auto screenNavigator = std::make_shared<ScreenNavigator<NavigationDriver>>(&mTft, menuFrame, 1000);
    auto& scenes = mpAppContext->getMQTTGroups();
    auto widgets = std::vector<WidgetPtr>();
    for (auto& scene : scenes)
    {
      std::visit(::util::overloaded([&](auto&& ptr)
      {
        auto button = util::UISceneButtonBuilder<decltype(this)>()(ptr, this);
        widgets.push_back(button);
      }), scene);
    }

    screenNavigator->addSubviews(widgets);
    mpSubViews.emplace_back(screenNavigator);
  }

  // Touch Driver Specialization
  template<class ScreenDriver, class NavigationDriver>
  template<class N>
  void AppScreen<ScreenDriver, NavigationDriver>::draw(typename std::enable_if<std::is_same<N, gfx::TouchDriver>::value, N>::type*)
  {
    std::lock_guard<std::mutex> guard(viewMutex);
    auto tapEvent = mNavigation.tapEvent();
    mScreenSaver();

    // Abort when Screensaver is on and no touch event happened.
    if (!mScreenSaver.tapped(tapEvent))
    {
      return;
    }

    mpStatusBar->draw();
    std::for_each(mpSubViews.begin(), mpSubViews.end(), [&](auto& subView) {
      if (mNeedsRedraw)
      {
        subView->draw();
      }
    });

    if (!tapEvent)
    {
      return;
    }
    std::for_each(mpSubViews.begin(), mpSubViews.end(), [&](auto& subView) {
        subView->didTap(*tapEvent);
    });

  }

  // Button Driver Specialization
  template<class ScreenDriver, class NavigationDriver>
  template<class N>
  void AppScreen<ScreenDriver, NavigationDriver>::draw(typename std::enable_if<std::is_same<N, gfx::ButtonDriver>::value, N>::type*)
  {
    std::lock_guard<std::mutex> guard(viewMutex);
    auto btnEvent = mNavigation.buttonEvent();
    mScreenSaver();

    // Abort when Screensaver is on and no touch event happened.
    if (!mScreenSaver.tapped(btnEvent))
    {
      return;
    }

    if (btnEvent)
    {
      std::for_each(mpSubViews.begin(), mpSubViews.end(), [&](auto& subView) {
            subView->didTap(*btnEvent);
      });
    }

    mpStatusBar->draw();
    std::for_each(mpSubViews.begin(), mpSubViews.end(), [&](auto& subView) {
      if (mNeedsRedraw)
      {
        subView->draw();
      }
    });
  }

  template<class ScreenDriver, class NavigationDriver>
  void AppScreen<ScreenDriver, NavigationDriver>::dismissPresentingScreen(const uint16_t sceneId)
  {
    mIsPresenting = false;
    std::thread changeViewThread([&]()
    {
      std::lock_guard<std::mutex> lock(viewMutex);
      mpSubViews.pop_back();
      presentMenu();
    });
    changeViewThread.detach();
    mpStatusBar->setTextLabel("");
  }

  template<class ScreenDriver, class NavigationDriver>
  void AppScreen<ScreenDriver, NavigationDriver>::showWarning(const std::string warningMessage)
  {
    std::lock_guard<std::mutex> guard(viewMutex);
    Frame frame {{0, kStatusBarHeight, 0}, mViewPortSize};
    auto warningWidget = std::make_shared<UIErrorWidget>(&mTft, frame, 99);
    warningWidget->setWarningMessage(warningMessage);
    mpSubViews.clear();
    mpSubViews.push_back(warningWidget);
  }
} // namespace gfx
