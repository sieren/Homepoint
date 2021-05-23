#pragma once
#include <mqtt/MQTTConnection.h>
#include <ntp/NTPSync.h>
#include "AppContext.h"
#include <ui/Color.hpp>
#include <ui/UIMosaicButton.h>
#include "lvgl.h"
#include <chrono>
#include <optional>
#include <vector>

namespace gfx
{
  class UISensorComboButton : public UIMosaicButton
  {
    public:
      using ImagePath = std::string;
      using ValueType = std::string;

      UISensorComboButton(lv_obj_t *parent, const std::string label, const uint16_t id, const bool isMultiSensor = false);
      void setTextLabel(const std::string label);
      void setupButton();
      void eraseValues();
      void updateImageWithValue(const std::vector<std::pair<ImagePath, ValueType>> valuePair);
      void addTargetAction(ButtonCallback cb);
      void addTargetLongPressAction(ButtonCallback cb);
  
    private:
      Color mTextColor;
      bool isMultiSensor = false;
      std::vector<std::pair<ImagePath, ValueType>> mValues;
      lv_obj_t* mpLabelText;
      lv_obj_t* mpSecondImage;
      lv_obj_t* mpFirstLabel;
      lv_obj_t* mpSecondLabel;
      std::string mLabel;
      std::optional<ButtonCallback> mButtonCallback;
      std::optional<ButtonCallback> mButtonLongPressCallback;
  };
} // namespace gfx
