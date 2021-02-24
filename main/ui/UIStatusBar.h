#pragma once
#include <mqtt/MQTTConnection.h>
#include <ntp/NTPSync.h>
#include "AppContext.h"
#include <ui/Color.hpp>
#include "lvgl.h"
#include <chrono>

namespace gfx
{
  enum class WIFIStatus {
    DISCONNECTED = 0,
    CONNECTED = 1
  };

  class UIStatusBar
  {
    public:
      UIStatusBar(lv_obj_t *parent);
      void registerCallback(ctx::WifiContext* ctx);
      void wifiChanged(ctx::WifiConnectionState connectionState);
      void mqttConnectionChanged(mqtt::MQTTConnectionStatus connectionState);
      void setTextColor(const Color color);
      void setTextLabel(const std::string label);

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
      lv_obj_t* mpStatusSymbol;
  };
} // namespace gfx
