#pragma once

#include <AppContext.h>
#include <config/PlatformInject.hpp>
#include <touch/TouchTypes.hpp>

#include <chrono>
#include <memory>
#include <optional>

namespace gfx
{
  template<typename ScreenDriver>
  struct ScreenSaver
  {
    public:
      ScreenSaver(ScreenDriver* driver, std::shared_ptr<ctx::AppContext> ctx) :
        mpDriver(driver),
        mpCtx(ctx)
      {
        mLastTouch = std::chrono::system_clock::now();
        // pinMode(TFT_LED, OUTPUT);
        // digitalWrite(TFT_LED, LOW);    // LOW to turn backlight on - pcb version 01-02-00
      }

      void operator()()
      {
        auto now = std::chrono::system_clock::now();
        const auto timeOut = mpCtx->getModel().mHardwareConfig.mScreensaverMins;
        if (std::chrono::duration_cast<std::chrono::minutes>(now - mLastTouch).count() > timeOut)
        {
          switchScreen(false);
        }
        else
        {
          switchScreen(true);
        }
      }

      template<typename T>
      bool tapped(const T& tapEvt)
      {
        if (!tapEvt)
        {
          return mCurrentState;
        }
        auto tapEvent = *tapEvt;
        if (tapEvent.state == decltype(tapEvent.state)::Tap)
        {
          mLastTouch = std::chrono::system_clock::now();
        }
        return mCurrentState;
      }

      void activate()
      {
        mLastTouch = std::chrono::system_clock::now();
      }

    private:

      void switchScreen(bool on)
      {
        if (on == mCurrentState)
        {
          return;
        }
        ScreenOnOffSwitch(mpDriver, on, mpCtx->getModel().mHardwareConfig.mIsLEDPinInverted);
        mCurrentState = on;
      }

      std::chrono::system_clock::time_point mLastTouch;
      bool mCurrentState = true;
      ScreenDriver* mpDriver;
      std::shared_ptr<ctx::AppContext> mpCtx;

  };
} // namespace gfx
