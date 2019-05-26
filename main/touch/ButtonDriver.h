#pragma once

#include "TFT_eSPI.h"
#include <touch/ButtonTypes.h>
#include <util/optional.hpp>

#include <chrono>

namespace gfx
{
  struct ButtonDriver
  {
    ButtonDriver(TFT_eSPI* tftDriver);

    tl::optional<ButtonEvent> buttonEvent();

    private:
      tl::optional<ButtonPressState> buttonState();

      std::chrono::system_clock::time_point mLastPressTime;
      ButtonPressState mCurrentPressState;
  };
} // namespace gfx
