#pragma once

#include "TFT_eSPI.h"
#include <touch/ButtonTypes.h>

#include <chrono>
#include <optional>

namespace gfx
{
  struct ButtonDriver
  {
    ButtonDriver(TFT_eSPI* tftDriver);

    std::optional<ButtonEvent> buttonEvent();

    private:
      std::optional<ButtonPressState> buttonState();

      std::chrono::system_clock::time_point mLastPressTime;
      ButtonPressState mCurrentPressState;
  };
} // namespace gfx
