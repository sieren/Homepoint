
#include "TouchDriver.h"
#include <config/Config.h>
#include <tft/ScreenSaver.hpp>

namespace gfx
{
  TouchDriver::TouchDriver(TFT_eSPI* tftDriver) :
    mTouch(tftDriver)
  {
    mCurrentEvent = {{0,0,0}, TouchState::NoTouch, xTaskGetTickCount()};
  };

  void TouchDriver::updateHardwareConfig(config::HardwareConfig& hwConfig)
  {
    mXAxisInversionAmount = hwConfig.mIsTouchXAxisInverted ? ScreenWidth : 0;
    mYAxisInversionAmount = hwConfig.mIsTouchYAxisInverted ? ScreenHeight : 0;
  }

  auto TouchDriver::touchPoint() -> std::optional<TouchEvent>
  {
    // TODO: Should probably average over 3 points with timestamps
    // because occasionally touches would "drop" out and lead
    // to Long Presses not being recognized correctly
    const auto now = xTaskGetTickCount();
    TouchState newState = TouchState::NoTouch;
    TouchEvent newEvent{{0,0,0}, TouchState::NoTouch, now};
    uint16_t x, y = 0;
    if (mTouch->getTouch(&x, &y, 100))
    {
      switch(mCurrentEvent.state)
      {
        case TouchState::NoTouch:
          newState = TouchState::TouchStart;
          break;
        case TouchState::TouchStart:
          newState = TouchState::TouchRunning;
          break;
        case TouchState::TouchEnded:
          break;
        case TouchState::TouchRunning:
          newState = TouchState::TouchRunning;
        default:
          break;
      }
      y = std::abs(mYAxisInversionAmount - y);
      x = std::abs(mXAxisInversionAmount - x);
      newEvent.position = {x, y, 0};
      newEvent.state = newState;
      mCurrentEvent = newEvent;
      return std::make_optional(mCurrentEvent);
    }
    else
    {
      switch(mCurrentEvent.state)
      {
        case TouchState::TouchRunning:
        case TouchState::TouchStart:
          newState = TouchState::TouchEnded;
          break;
        case TouchState::TouchEnded:
          newState = TouchState::NoTouch;
          break;
        default:
          newState = TouchState::NoTouch;
          break;
      }
      
      if (newState != mCurrentEvent.state)
      {
        mCurrentEvent.state = newState;
        return std::make_optional(mCurrentEvent);
      }
    }
    return std::nullopt;
  }


  auto TouchDriver::tapEvent() -> std::optional<TapEvent>
  {
    auto touchEvt = touchPoint();
    if (!touchEvt)
    {
      return std::nullopt;
    }
    auto evt = *touchEvt;
    if (evt.state == TouchState::TouchStart)
    {
      mLastTouchEventTime = evt.timestamp;
    }
    const auto pressDelta = xTaskGetTickCount() - mLastTouchEventTime;
    const auto isValid = pressDelta < MSBeforeInvalid;
    if (!isValid)
    {
      return std::nullopt;
    }
    const auto isLongPress =  pressDelta > MSBeforeLongPress;
    const auto isShortPress = pressDelta < MSBeforeTap;

    if (evt.state == TouchState::TouchEnded)
    {
      TapEvent tapEvent;
      tapEvent.position = evt.position;
      if (isShortPress)
      {
        tapEvent.state = PressEvent::Tap;
        return std::make_optional(tapEvent);
      }
      if (isLongPress)
      {
        tapEvent.state = PressEvent::LongPress;
        return std::make_optional(tapEvent);
      }
    }
    return std::nullopt;
  }
} // namespace gfx
