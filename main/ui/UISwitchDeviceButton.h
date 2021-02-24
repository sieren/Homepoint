#pragma once
#include <mqtt/MQTTConnection.h>
#include <ntp/NTPSync.h>
#include "AppContext.h"
#include <ui/Color.hpp>
#include <ui/UIMosaicButton.h>
#include "lvgl.h"
#include <chrono>
#include <optional>

namespace gfx
{
  class UISwitchDeviceButton : public UIMosaicButton
  {
    public:
      UISwitchDeviceButton(lv_obj_t *parent, const std::string label, const uint16_t sceneId);
      void setTextLabel(const std::string label);
      void setupButton();
      void addTargetAction(ButtonCallback cb);
      void addTargetLongPressAction(ButtonCallback cb);
  
    private:
      Color mTextColor;
      lv_obj_t* mpLabelText;
      std::string mLabel;
      std::optional<ButtonCallback> mButtonCallback;
      std::optional<ButtonCallback> mButtonLongPressCallback;
  };
} // namespace gfx
