#pragma once

#include <touch/ButtonTypes.h>

#include <chrono>
#include <optional>

class TFT_eSPI;
namespace config { struct HardwareConfig; }

namespace gfx
{
  struct ButtonDriver
  {
    ButtonDriver(TFT_eSPI* tftDriver);
    void updateHardwareConfig(config::HardwareConfig& hwConfig) { }; // noop

    std::optional<ButtonEvent> buttonEvent();

    private:
      std::optional<ButtonPressState> buttonState();

      std::chrono::system_clock::time_point mLastPressTime;
      ButtonPressState mCurrentPressState;
  };
} // namespace gfx
