#pragma once

#include "TFT_eSPI.h"
#include <touch/ButtonTypes.h>

#include <chrono>
#include <optional>

namespace config { struct HardwareConfig; }

namespace gfx
{
  struct ButtonDriver
  {
    ButtonDriver(TFT_eSPI* tftDriver, config::HardwareConfig& hwConfig);

    std::optional<ButtonEvent> buttonEvent();

    private:
      std::optional<ButtonPressState> buttonState();

      std::chrono::system_clock::time_point mLastPressTime;
      ButtonPressState mCurrentPressState;
  };
} // namespace gfx
