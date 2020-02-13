#pragma once
#include <mqtt/MQTTConnection.h>
#include <ntp/NTPSync.h>
#include "UIWidget.hpp"
#include "AppContext.h"

#include <chrono>

namespace gfx
{
  enum class WIFIStatus {
    DISCONNECTED = 0,
    CONNECTED = 1
  };
  class UIStatusBarWidget : public UIWidget
  {
    public:
      UIStatusBarWidget(ScreenDriver* screenptr, Frame frame, uint16_t tag = 0);
      void registerCallback(ctx::WifiContext* ctx);
      void wifiChanged(ctx::WifiConnectionState connectionState);
      void mqttConnectionChanged(mqtt::MQTTConnectionStatus connectionState);
      void setTextColor(const Color color);
      void setTextLabel(const std::string label);

      void draw() override;
    private:
      ctx::WifiConnectionState mWifiState;
      Color mTextColor;
      std::string mIpAddressLabel = "N/A";
      std::string mWifiImage = "wifi_off";
      std::string mMqttImage = "mqtt_off";
      std::string mTimeLabel = "0:00";
      std::string mTextLabel = "";
      std::chrono::system_clock::time_point mLastUpdate;
      bool mIsWifiDisplayed = false;
  };
} // namespace gfx
