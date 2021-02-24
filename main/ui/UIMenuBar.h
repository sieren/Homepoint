#pragma once
#include <mqtt/MQTTConnection.h>
#include <ntp/NTPSync.h>
#include "AppContext.h"
#include <util/dispatcher.hpp>
#include <ui/Color.hpp>
#include "lvgl.h"
#include <chrono>

namespace gfx
{
  class UIMenuBar
  {
    public:
      UIMenuBar(lv_obj_t *parent);
      ~UIMenuBar();
      

    private:
      ctx::WifiConnectionState mWifiState;
      Color mTextColor;
      std::string mIpAddressLabel = "N/A";
      std::string mWifiImage = LV_SYMBOL_WARNING;
      std::string mMqttImage = "mqtt_off";
      std::string mTimeLabel = "0:00";
      std::string mTextLabel = "";
      std::chrono::system_clock::time_point mLastUpdate;
      bool mIsWifiDisplayed = false;
      lv_obj_t* mpParent;
      lv_obj_t* mpContHeader;
      lv_obj_t* mpStatusLabel;
      lv_obj_t* mpExitButton;
  };
} // namespace gfx
