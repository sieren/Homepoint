#include "MQTTConnection.h"
#include <mqtt/MQTTStateUpdater.hpp>
#include <util/varianthelper.hpp>

extern "C" 
{
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "esp_event_loop.h"
}

#include <algorithm>

static const char* TAG = "MQTT";

namespace mqtt
{
  MQTTConnection::MQTTConnection(MQTTConfig config, std::vector<DeviceVariants> mqttscenes) :
    mConfig(config),
    mMQTTScenes(mqttscenes)
  {

  }

  void MQTTConnection::eventCallback(esp_mqtt_event_handle_t event)
  {
    switch (event->event_id) {
        case MQTT_EVENT_BEFORE_CONNECT:
            mLastState = MQTTConnectionStatus::CONNECTING;
            mConnectionStatusNotifier.broadcast(mLastState);
          break;
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            mLastState = MQTTConnectionStatus::CONNECTED;
            mConnectionStatusNotifier.broadcast(mLastState);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            mLastState = MQTTConnectionStatus::DISCONNECTED;
            mConnectionStatusNotifier.broadcast(mLastState);
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            updateScenes(event);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;
    }
  }

  void MQTTConnection::connect()
  {
    esp_mqtt_client_config_t mqtt_cfg = { };
    mqtt_cfg.uri = mConfig.addr.c_str();
    mqtt_cfg.username = std::get<0>(mConfig.credentials).c_str();
    mqtt_cfg.password = std::get<1>(mConfig.credentials).c_str();
    mqtt_cfg.event_handle = [](esp_mqtt_event_handle_t event) -> int
    {
      (*static_cast<decltype(this)>(event->user_context)).eventCallback(event);
      return ESP_OK;
    };
    mqtt_cfg.user_context = this;

    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_start(client);
  }

  void MQTTConnection::bindScenes()
  {
    for (auto& scene : mMQTTScenes)
    {
      mpark::visit(::util::overloaded([&](MQTTSwitchGroupPtr ptr)
        {
          for (auto device : ptr->mDevices)
          {
            esp_mqtt_client_subscribe(client, device.second.getTopic.c_str(), 0);
          }
        },
        [&](MQTTSensorGroupPtr ptr)
        {
          for (auto device : ptr->mSensorDevices)
          {
            esp_mqtt_client_subscribe(client, device.second.getTopic.c_str(), 0);
          }
        },
        [&](auto& i) {}), scene);
    }
  }

  void MQTTConnection::switchScene(const uint16_t id, bool on)
  {
    auto scene = std::find_if(mMQTTScenes.begin(), mMQTTScenes.end(), [&id](auto& ele)
    {
      return mpark::visit(::util::overloaded([&id](MQTTSwitchGroupPtr&& elem)
      {
        return elem->groupId == id;
      },
      [](auto&& i) { return false; }), ele);
    });
    if (scene != mMQTTScenes.end())
    {
      mpark::visit(::util::overloaded([&](MQTTSwitchGroupPtr ptr)
      {
        for (auto& device : ptr->mDevices)
        {
          const auto onValue = on ? device.second.onValue : device.second.offValue;
          esp_mqtt_client_publish(client, device.second.setTopic.c_str(), onValue.c_str(), 0, 0, 0);
          device.second.active = on;
        }
        if (ptr->mSetNeedsUpdateCB)
        {
          ptr->mSetNeedsUpdateCB();
        }
      },
      [](auto&& i) {}), *scene);
    }
  }

  void MQTTConnection::switchDevice(const uint16_t groupid, const uint16_t deviceid, bool on)
  {
    ESP_LOGI(TAG, "Switching, grp=%d, dev=%d", groupid, deviceid);
    auto scene = std::find_if(mMQTTScenes.begin(), mMQTTScenes.end(), [&groupid](auto& ele)
    {
      return mpark::visit(::util::overloaded([&groupid](MQTTSwitchGroupPtr&& elem)
      {
        return elem->groupId == groupid;
      },
      [](auto&& i) { return false; }), ele);
    });

    if (scene != mMQTTScenes.end())
    {
      mpark::visit(::util::overloaded([&](MQTTSwitchGroupPtr ptr)
      {
        auto device = std::find_if(ptr->mDevices.begin(), ptr->mDevices.end(), [&deviceid](auto& ele)
        {
          return ele.second.deviceId == deviceid;
        });

        if (device == ptr->mDevices.end())
        {
          return;
        }
        const auto onValue = on ? device->second.onValue : device->second.offValue;
        esp_mqtt_client_publish(client, device->second.setTopic.c_str(), onValue.c_str(), 0, 0, 0);
      },
      [](auto&& i) {}), *scene);
    }
  }

  void MQTTConnection::updateScenes(esp_mqtt_event_handle_t event)
  {
    for (auto& scene : mMQTTScenes)
    {
      mpark::visit(::util::overloaded(
        [event](auto&& ptr)
      { 
        mqtt::MQTTStateUpdater()(ptr, event);
      }),
      scene);
    }
  }

  void MQTTConnection::registerConnectionStatusCallback(MQTTConnectionCB cb)
  {
    mConnectionStatusNotifier.addCB(cb);
  }

  MQTTConnectionStatus MQTTConnection::getLastState()
  {
    return mLastState;
  }
} // namespace mqtt
