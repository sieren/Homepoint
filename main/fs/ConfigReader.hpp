#pragma once

#include <util/warnings.h>
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include <rapidjson/prettywriter.h>
#include "Filesystem.h"
#include <model/Model.hpp>

extern "C" 
{
  #include "esp_log.h"
}

namespace fs
{
class ConfigReader
{
  public:
    const model::Model readConfiguration()
    {
      model::Model model;
      using namespace rapidjson;
      auto config = fs::FileSystem::getInstance().readJsonConfig("/spiffs/config.json");
      const char* configChar = config.c_str();
      Document document;
      ParseResult res = document.Parse<0>(configChar);
      if (!res)
      {
        throw std::runtime_error("Could not parse config file!");
      }

      model.mWifiCredentials = ConfigReader::getWifiCredentials(document);
      model.mWebCredentials = ConfigReader::getWebCredentials(document);
      model.mTimeZone = getTimeZone(document);
      model.mMQTTServerConfig = getMQTTConfig(document);
      model.mMQTTGroups = getMQTTGroups(document);
      return model;
    }

    const model::Model readFailsafeConfiguration()
    {
      model::Model model;
      using namespace rapidjson;
      auto config = fs::FileSystem::getInstance().readJsonConfig("/spiffs/failsafe.json");
      Document document;
      ParseResult res = document.Parse<0>(config.c_str());
      if (!res)
      {
        throw std::runtime_error("Could not parse failsafe file!");
      }
      model.mWifiCredentials = ConfigReader::getWifiCredentials(document);
      model.mWebCredentials = ConfigReader::getWebCredentials(document);
      return model;
    }

  void setFirstLaunch(const WifiCredentials credentials,
    std::string login, std::string password)
    {
      using namespace rapidjson;
      using DocuType = rapidjson::GenericDocument<rapidjson::UTF8<>,rapidjson::CrtAllocator>;
      using ElemType = rapidjson::GenericValue<rapidjson::UTF8<>,rapidjson::CrtAllocator>;
      DocuType document;

      ESP_LOGI("CONFIG READER", "Creating DOM");

      document.SetObject();
      ElemType ssidName(kStringType);
      ssidName.SetString(std::get<0>(credentials).c_str(), std::get<0>(credentials).length(), document.GetAllocator());
      document.AddMember("wifi", ssidName.Move(), document.GetAllocator());

      ElemType wifiPassword(kStringType);
      wifiPassword.SetString(std::get<1>(credentials).c_str(), std::get<1>(credentials).length(), document.GetAllocator());
      document.AddMember("password", wifiPassword.Move(), document.GetAllocator());

      ElemType webLogin(kStringType);
      webLogin.SetString(login.c_str(), login.length(), document.GetAllocator());
      document.AddMember("login", webLogin.Move(), document.GetAllocator());

      ElemType webPass(kStringType);
      webPass.SetString(password.c_str(), password.length(), document.GetAllocator());
      document.AddMember("webpass", webPass.Move(), document.GetAllocator());

      ESP_LOGI("CONFIG READER", "Writing JSON String Output");
      rapidjson::StringBuffer buffer;
      buffer.Clear();

      rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
      document.Accept(writer);
      const auto content = std::string(buffer.GetString());
      ESP_LOGI("CONFIG READER", "Saving to file");
      fs::FileSystem::getInstance().writeJsonConfig("/spiffs/config.json", content);
      fs::FileSystem::getInstance().writeJsonConfig("/spiffs/failsafe.json", content);
    }

  private:
    const WifiCredentials getWifiCredentials(rapidjson::Document& document)
    {
      using namespace rapidjson;
      Value& apName = document["wifi"];
      Value& passWd = document["password"];
      return std::make_tuple(apName.GetString(), passWd.GetString());
    }

    const WifiCredentials getWebCredentials(rapidjson::Document& document)
    {
      using namespace rapidjson;
      Value& webLogin = document["login"];
      Value& webPassword = document["webpass"];
      return std::make_tuple(webLogin.GetString(), webPassword.GetString());
    }

    const std::string getTimeZone(rapidjson::Document& document)
    {
      using namespace rapidjson;
      if (!document.HasMember("timezone"))
      {
        return "";
      }
      Value& timeZone = document["timezone"];
      return std::string(timeZone.GetString());
    }

    const mqtt::MQTTConfig getMQTTConfig(rapidjson::Document& document)
    {
      mqtt::MQTTConfig conf;
      using namespace rapidjson;
      if (!document.HasMember("mqttbroker"))
      {
        return {};
      }
      Value& ipAddr = document["mqttbroker"];
      Value& mqttusername = document["mqttusername"];
      Value& mqttpasswd = document["mqttpasswd"];
      conf.credentials = std::make_tuple(mqttusername.GetString(), mqttpasswd.GetString());
      conf.addr = ipAddr.GetString();
      return conf;
    }

    static std::vector<MQTTVariants> getMQTTGroups(rapidjson::Document& document)
    {
      using namespace rapidjson;
      using namespace mqtt;

      uint16_t tagId = 0;
      if (!document.HasMember("scenes"))
      {
        return {};
      }
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
