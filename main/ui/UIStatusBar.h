#pragma once
#include <mqtt/MQTTConnection.h>
#include <ntp/NTPSync.h>
#include "AppContext.h"
#include <ui/ActionCallback.hpp>
#include <util/dispatcher.hpp>
#include <ui/Color.hpp>
#include "lvgl.h"

#include <memory>
#include <optional>
#include <chrono>

namespace gfx
{
  enum class WIFIStatus {
    DISCONNECTED = 0,
    CONNECTED = 1
  };

  class UIStatusBar //: public std::enable_shared_from_this<UIStatusBar>
  {
    public:
      UIStatusBar(lv_obj_t *parent, std::shared_ptr<ctx::WifiContext> ctx, std::shared_ptr<mqtt::MQTTConnection> mqttCtx);
      ~UIStatusBar();
      void setupScreen(lv_obj_t* parent);
      void setupCallbacks();
      // std::shared_ptr<UIStatusBar> getShared(){
      //   return shared_from_this();
      // }
      void setAction(MenuAction action);
      void registerCallback(ctx::WifiContext* ctx);
      void registerMQTTCallback(mqtt::MQTTConnection* mqttCtx);
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
      Dispatcher<ctx::WifiConnectionState>::CBID callbackId;
      Dispatcher<mqtt::MQTTConnectionStatus>::CBID mqttCallbackId;
      std::optional<ActionCallback> mActionCallback;
      bool mIsWifiDisplayed = false;
      lv_obj_t* mpParent;
      lv_obj_t* mpContHeader;
      lv_obj_t* mpStatusLabel;
      lv_obj_t* mpActionLabel;
      lv_obj_t* mpStatusSymbol;
      std::shared_ptr<ctx::WifiContext> mpWifiContext;
      std::shared_ptr<mqtt::MQTTConnection> mpMQTTContext;
  };
} // namespace gfx
