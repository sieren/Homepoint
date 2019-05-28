#pragma once

#include <AppContext.h>
#include <config/Config.h>
#include <tft/ScreenSaver.hpp>
#include <ui/UIDetailButtonBuilder.hpp>
#include <ui/UIStatusBarWidget.h>
#include <ui/UIWidget.hpp>
#include <ui/UIWidgetBuilder.hpp>
#include <ui/UIPosition.hpp>

#include <memory>
#include <mutex>

namespace gfx
{
template<class ScreenDriver, class NavigationDriver>
class AppScreen : public UIWidget
{
  public:
    AppScreen(std::shared_ptr<ctx::AppContext> ctx, Size size = {320, 240});

    AppScreen() = default;
    void setupScreen();
    void setupData();

    template<class N = NavigationDriver>
    void draw(typename std::enable_if<std::is_same<N, gfx::TouchDriver>::value, N >::type* = 0);
    template<class N = NavigationDriver>
    void draw(typename std::enable_if<std::is_same<N, gfx::ButtonDriver>::value, N >::type* = 0);

    void showWarning(const std::string warningMessage);

    void presentMenu();
    void presentScreen(const uint16_t sceneId);
    void dismissPresentingScreen(const uint16_t tag);
    void didTouch(const TouchEvent& evt);

  private:
    template<class>
    friend struct util::UISceneButtonBuilder;
    template<class>
    friend struct util::UIDetailButtonBuilder;
    void clearScreen();
    bool mIsPresenting{false};
    Size mWindowSize;
    Size mViewPortSize;
    const Frame menuFrame;
    ScreenDriver mTft;
    NavigationDriver mNavigation;
    std::shared_ptr<ctx::AppContext> mpAppContext;
    std::shared_ptr<UIStatusBarWidget> mpStatusBar;
    ScreenSaver<ScreenDriver> mScreenSaver;
    std::mutex viewMutex;
};

} // namespace gfx

#include "AppScreen.ipp"
