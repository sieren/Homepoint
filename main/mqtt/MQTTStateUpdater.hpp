#pragma once

#include "MQTTGroup.hpp"
#include "MQTTSensorGroup.hpp"

#include <map>

extern "C"
{
  #include "mqtt_client.h"
  #include "esp_log.h"
}

namespace mqtt
{
  struct MQTTStateUpdater
  {
    void operator()(MQTTSwitchGroupPtr switchGroup, esp_mqtt_event_handle_t evt)
    {
      const auto topic = std::string(evt->topic, evt->topic_len);
      auto searchDevice = std::find_if(switchGroup->mDevices.begin(), switchGroup->mDevices.end(),
        [topic](auto& device)
        {
          return device.first == topic;
        }
      ); 

      if (searchDevice == switchGroup->mDevices.end())
      {
        return;
      }

      const auto topicData = std::string(evt->data, evt->data_len);
      auto& device = searchDevice->second;
      device.active = topicData == device.onValue ? true : false;
      if (device.mSetNeedsUpdateCB)
      {
        device.mSetNeedsUpdateCB();
      }
      if (switchGroup->mSetNeedsUpdateCB)
      {
        switchGroup->mSetNeedsUpdateCB();
      }
    }

    void operator()(MQTTSensorGroupPtr switchGroup, esp_mqtt_event_handle_t evt)
    {
      const auto topic = std::string(evt->topic, evt->topic_len);
      auto searchDevice = std::find_if(switchGroup->mSensorDevices.begin(), switchGroup->mSensorDevices.end(),
        [topic](auto& device)
        {
          return device.first == topic;
        }
      ); 

      if (searchDevice == switchGroup->mSensorDevices.end())
      {
        return;
      }

      const auto topicData = std::string(evt->data, evt->data_len);
      auto& device = searchDevice->second;
      device.setValue(topicData);
      if (switchGroup->mSetNeedsUpdateCB)
      {
        switchGroup->mSetNeedsUpdateCB();
      }
    }

    void operator()(auto i, esp_mqtt_event_handle_t evt)
    {
      // NO OP
      // IF YOU RUN INTO THIS,
      // ADD A HANDLER FOR A NEW SCENE TYPE
    };
  };
}