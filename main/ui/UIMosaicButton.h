#pragma once
#include <mqtt/MQTTConnection.h>
#include <ntp/NTPSync.h>
#include "AppContext.h"
#include <ui/Color.hpp>
#include "lvgl.h"
#include <chrono>
#include <optional>

namespace gfx
{
  class UIMosaicButton;
  using MosaicButtonPtr = std::shared_ptr<UIMosaicButton>;
  using ButtonCallback = std::function<void(uint16_t)>;
  class UIMosaicButton
  {
    public:
      UIMosaicButton(lv_obj_t *parent, const std::string label, const uint16_t tagId = 0);
      void setupButton();
      void setState(const mqtt::MQTTSwitchGroupState state);
      void setState(const bool isActive);
      void setTextLabel(const std::string label);
      void setImage(const std::string imageName);
      void setImageActive(bool isActive);
      void addTargetAction(ButtonCallback cb);
      void addTargetLongPressAction(ButtonCallback cb);
      uint16_t getId();
  
    protected:
      Color mTextColor;
      lv_obj_t* mpParent;
      lv_obj_t* mpContHeader;
      lv_obj_t* mpLabelText;
      lv_obj_t* mpImage;
      uint16_t mTagId;
      std::string mLabel;
      std::optional<ButtonCallback> mButtonCallback;
      std::optional<ButtonCallback> mButtonLongPressCallback;
  };
} // namespace gfx
