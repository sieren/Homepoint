#pragma once

#include <config/Config.h>

namespace config
{
  // Hardware Configuration that can be overwritten during runtime
  struct HardwareConfig
  {
    bool mIsLEDPinInverted = kLED_PIN_INVERTED;
    int mScreensaverMins = 10;
    int mScreenRotationAngle = SCREEN_ROTATION_ANGLE;
    bool mIsTouchXAxisInverted = kTOUCH_X_AXIS_INVERTED;
    bool mIsTouchYAxisInverted = kTOUCH_Y_AXIS_INVERTED;
    bool mIsDisplayColorInverted = kDISPLAY_INVERTED;
  };
}