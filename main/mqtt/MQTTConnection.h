#pragma once

#include <devices/DeviceTypes.hpp>
#include <util/dispatcher.hpp>

#include <functional>
#include <string>
#include <tuple>

extern "C"
{
#include "mqtt_client.h"
}

namespace mqtt
{
  using MQTTCredentials = std::tuple<std::string, std::string>;
  using MQTTHTTPAddress = std::string;

  enum class MQTTConnectionStatus: int
  {
    DISCONNECTED = 0,
    CONNECTED = 1,
    CONNECTING = 2
  };

  using MQTTConnectionCB = std::function<void(MQTTConnectionStatus)>;

  struct MQTTConfig
  {
    MQTTCredentials credentials;
    MQTTHTTPAddress addr;
  };

  class MQTTConnection
  {
    public:
      MQTTConnection(MQTTConfig config, std::vector<DeviceVariants> mqttscenes);
      void connect();
      void bindScenes();
      void switchScene(const uint16_t id, bool on);
      void switchDevice(const uint16_t groupid, const uint16_t deviceid, bool on);
      void eventCallback(esp_mqtt_event_handle_t event);
      MQTTConnectionStatus getLastState();

      void registerConnectionStatusCallback(MQTTConnectionCB cb);
    private:
      void updateScenes(esp_mqtt_event_handle_t event);
      MQTTConfig mConfig;
      esp_mqtt_client_handle_t client;
      std::vector<DeviceVariants> mMQTTScenes;
      MQTTConnectionStatus mLastState;
      Dispatcher<MQTTConnectionStatus> mConnectionStatusNotifier;
  };
} // namespace mqtt
