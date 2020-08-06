#include <touch/ButtonDriver.h>
#include <config/Config.h>

// Specifically for M5Stack which uses Pull-Ups on buttons

namespace gfx
{

  ButtonDriver::ButtonDriver(TFT_eSPI* tftDriver)
  {
    if (ButtonsArePullUp)
    {
      pinMode(BUTTON_A_PIN, INPUT_PULLUP);
      pinMode(BUTTON_B_PIN, INPUT_PULLUP);
      pinMode(BUTTON_C_PIN, INPUT_PULLUP);
    }

    mCurrentPressState.state = PressState::None;
  }
  
  std::optional<ButtonEvent> ButtonDriver::buttonEvent()
  {
    auto now = std::chrono::system_clock::now();
    auto button = buttonState();
    if (!button)
    {
      return std::nullopt;
    }
    auto btn = *button;
    if (btn.state == PressState::Start)
    {
      mLastPressTime = std::chrono::system_clock::now();
    }
    const auto pressDelta = std::chrono::duration_cast<std::chrono::milliseconds>(now - mLastPressTime).count();
    const auto isValid = pressDelta < MSBeforeInvalid;
    if (!isValid)
    {
      return std::nullopt;
    }
    const auto isLongPress = pressDelta > MSBeforeLongPress;
    const auto isShortPress = pressDelta < MSBeforeTap;
    if (btn.state == PressState::Ended)
    {
      ButtonEvent btnEvent;
      btnEvent.button = btn.btn;
      if (isShortPress)
      {
        btnEvent.state = ButtonPress::Tap;
        return std::make_optional(btnEvent);
      }
      if (isLongPress)
      {
        btnEvent.state = ButtonPress::LongPress;
        return std::make_optional(btnEvent);
      }
    }
    return std::nullopt;
  }

  std::optional<ButtonPressState> ButtonDriver::buttonState()
  {
    Button btn = digitalRead(BUTTON_A_PIN) == 0 ? Button::A : Button::NONE;
    btn = digitalRead(BUTTON_B_PIN) == 0 ? Button::B : btn;
    btn = digitalRead(BUTTON_C_PIN) == 0 ? Button::C : btn;
    ButtonPressState newState;
    if (btn != Button::NONE)
    {
      switch (mCurrentPressState.state)
      {
        case PressState::None:
          newState.state = PressState::Start;
          break;
        case PressState::Start:
          newState.state = PressState::Running;
        case PressState::Running:
          break;
        case PressState::Ended:
          newState.state = PressState::Start;
          break;
      }
      mCurrentPressState = newState;
      mCurrentPressState.btn = btn;
      return std::make_optional(mCurrentPressState);
    }
    else
    {
      switch (mCurrentPressState.state)
      {
        case PressState::None:
          break;
        case PressState::Start:
        case PressState::Running:
          newState.state = PressState::Ended;
          break;
        case PressState::Ended:
          newState.state = PressState::None;
          break;
        default:
          newState.state = PressState::None;
          break;
      }
      if (newState.state == PressState::Ended)
      {
        ButtonPressState evt;
        evt.btn = mCurrentPressState.btn;
        evt.state = newState.state;
        mCurrentPressState.state = newState.state;
        return std::make_optional(evt);
      }
      mCurrentPressState = newState;
      return std::nullopt;
    }
  }
} // namespace gfx
