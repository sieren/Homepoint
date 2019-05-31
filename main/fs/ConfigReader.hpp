#pragma once

#include <util/warnings.h>
#include "rapidjson/document.h"
#include "Filesystem.h"
#include <mqtt/MQTTConnection.h>
#include <mqtt/MQTTGroup.hpp>
#include <mqtt/MQTTSensorGroup.hpp>

extern "C" 
{
#include "esp_log.h"
}

#include <map>
#include <string>
#include <tuple>
#include <vector>

using WifiCredentials = std::tuple<std::string, std::string>;

namespace fs
{
  class ConfigReader
  {
    public:
      static const WifiCredentials getWifiCredentials()
      {
        using namespace rapidjson;
        auto config = fs::FileSystem::getInstance().readJsonConfig("/spiffs/config.json");
        const char* configChar = config.c_str();
        Document document;
        document.Parse<0>(configChar);
        Value& apName = document["wifi"];
        Value& passWd = document["password"];
        return std::make_tuple(apName.GetString(), passWd.GetString());
      }

      static const std::string getTimeZone()
      {
        using namespace rapidjson;
        auto config = fs::FileSystem::getInstance().readJsonConfig("/spiffs/config.json");
        const char* configChar = config.c_str();
        Document document;
        document.Parse<0>(configChar);
        if (!document.HasMember("timezone"))
        {
          return "";
        }
        Value& timeZone = document["timezone"];
        return std::string(timeZone.GetString());
      }

      static const mqtt::MQTTConfig getMQTTConfig()
      {
        mqtt::MQTTConfig conf;
        using namespace rapidjson;
        auto config = fs::FileSystem::getInstance().readJsonConfig("/spiffs/config.json");
        const char* configChar = config.c_str();
        Document document;
        document.Parse<0>(configChar);
        Value& ipAddr = document["mqttbroker"];
        Value& mqttusername = document["mqttusername"];
        Value& mqttpasswd = document["mqttpasswd"];
        conf.credentials = std::make_tuple(mqttusername.GetString(), mqttpasswd.GetString());
        conf.addr = ipAddr.GetString();
        return conf;
      }

      static const homekit::HKConfig getHKConfig()
      {
        mqtt::MQTTConfig conf;
        using namespace rapidjson;
        auto config = fs::FileSystem::getInstance().readJsonConfig("/spiffs/config.json");
        const char* configChar = config.c_str();
        Document document;
        document.Parse<0>(configChar);

        if (document.HasMember("homekitpin"))
        {
          Value& hkPin = document["homekitpin"];
          return homekit::HKConfig{hkPin.GetString(), true};;
        }
        return homekit::HKConfig{std::string(""), false};
      }
      {
        using namespace rapidjson;
        using namespace mqtt;
        auto config = fs::FileSystem::getInstance().readJsonConfig("/spiffs/config.json");
        const char* configChar = config.c_str();
        Document document;
        document.Parse<0>(configChar);

        uint16_t tagId = 0;
        const auto& scenes = document["scenes"].GetArray();
        std::vector<MQTTVariants> vecScenes;
        for (const auto& scene : scenes)
        {
          if (std::string(scene["type"].GetString()) == "Light" ||
              std::string(scene["type"].GetString()) == "Switch")
          {
            auto aScene = std::make_shared<MQTTSwitchGroup>();
            aScene->sceneName = scene["name"].GetString();
            aScene->iconName = scene["icon"].GetString();
            GroupDevices<MQTTSwitchDevice> devices;
            int deviceId = 0;
            for (const auto& device : scene["devices"].GetArray())
            {
              MQTTSwitchDevice switchDevice;
              switchDevice.deviceId = deviceId;
              switchDevice.name = device["name"].GetString();
              switchDevice.getTopic = device["getTopic"].GetString();
              switchDevice.setTopic = device["setTopic"].GetString();
              switchDevice.onValue = device["onValue"].GetString();
              switchDevice.offValue = device["offValue"].GetString();
              devices.push_back({switchDevice.getTopic, std::move(switchDevice)});
              deviceId += 1;
            }
            aScene->mDevices = devices;
            aScene->groupId = tagId;
            MQTTVariants variant = aScene;
            vecScenes.push_back(aScene);
          }
          else if (std::string(scene["type"].GetString()) == "Sensor")
          {
            auto aScene = std::make_shared<MQTTSensorGroup>();
            aScene->sceneName = scene["name"].GetString();
            aScene->iconName = scene["icon"].GetString();
            GroupDevices<MQTTSensorDevice> devices;
            int deviceId = 0;
            for (const auto& device : scene["devices"].GetArray())
            {
              MQTTSensorDevice sensorDevice;
              sensorDevice.deviceId = deviceId;
              const auto sensorType = mqtt::util::GetSensorType(std::string(device["type"].GetString()));
              if (sensorType == MQTTSensorType::MQTTINVALID)
              {
                ESP_LOGI("CONFIG READER", "Unknown Sensor Type detected");
                abort();
              }
              sensorDevice.sensorType = sensorType;

              sensorDevice.dataType = MQTTSensorDataType::Raw;
              
              if (device.HasMember("jsondata"))
              {
                sensorDevice.dataType = device["jsondata"].GetBool() ? MQTTSensorDataType::JSON : MQTTSensorDataType::Raw;
              }

              if (sensorDevice.dataType == MQTTSensorDataType::JSON)
              {
                std::vector<ValueTuple> vals;
                if (sensorDevice.sensorType == MQTTSensorType::MQTTTemperatureHumidity)
                {
                  ValueTuple temperatureTuple {MQTTTemperatureKeyJSON, std::string(device[MQTTTemperatureKeyJSON.c_str()].GetString()), std::string("0")};
                  ValueTuple humidityTuple {MQTTHumidityKeyJSON, std::string(device[MQTTHumidityKeyJSON.c_str()].GetString()), std::string("0")};
                  vals.push_back(std::move(temperatureTuple));
                  vals.push_back(std::move(humidityTuple));
                  if (std::distance(scene["devices"].GetArray().begin(), scene["devices"].GetArray().end()) > 1)
                  {
                    ESP_LOGI("CONFIG READER", "Only one Temperature Humidty JSON Device per Scene allowed");
                    abort();
                  }
                }
                else
                {
                  const auto sensorType = MQTTSensorTypeJSONKey(sensorDevice.sensorType);
                  ValueTuple mappedValues {sensorType, std::string(device[sensorType.c_str()].GetString()), std::string("0")};
                  vals.push_back(std::move(mappedValues));
                }
                sensorDevice.mappedValues = vals;
              }
              sensorDevice.name = device["name"].GetString();
              sensorDevice.getTopic = device["getTopic"].GetString();
              devices.push_back({sensorDevice.getTopic, std::move(sensorDevice)});
              deviceId += 1;
            }
            aScene->mSensorDevices = devices;
            aScene->groupId = tagId;
            MQTTVariants variant = aScene;
            vecScenes.push_back(aScene);
          }
          tagId += 1;
        }
        return vecScenes;
      }

  };
} // namespace fs
