#include <fs/ConfigReader.hpp>
#include <fs/JsonHelper.hpp>

extern "C" 
{
  #include "esp_log.h"
}

namespace fs
{

    template<typename ConsumerFc>
    void read(const rapidjson::Value::ConstObject parent, const char* name, ConsumerFc consumer)
    {
      using T = json::ArgType<ConsumerFc>;
      json::PropertyFinder<T>().read(parent, name, consumer);
    }

    const model::Model ConfigReader::readConfiguration()
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
      const Document doc(std::move(document));
      const auto rootObj = doc.GetObject();
      model.mWifiCredentials = ConfigReader::getWifiCredentials(rootObj);
      model.mWebCredentials = ConfigReader::getWebCredentials(rootObj);
      model.mTimeZone = getTimeZone(rootObj);
      model.mMQTTServerConfig = getMQTTConfig(rootObj);
      model.mMQTTGroups = getMQTTGroups(rootObj);
      return model;
    }

    const model::Model ConfigReader::readFailsafeConfiguration()
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
      const Document doc(std::move(document));
      const auto rootObj = doc.GetObject();
      model.mWifiCredentials = ConfigReader::getWifiCredentials(rootObj);
      model.mWebCredentials = ConfigReader::getWebCredentials(rootObj);
      return model;
    }

  void ConfigReader::setFirstLaunch(const WifiCredentials credentials,
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

    const WifiCredentials ConfigReader::getWifiCredentials(const rapidjson::Value::ConstObject document)
    {
      using namespace rapidjson;
      std::string ssid;
      std::string password;
      read(document, "wifi", [&ssid] (std::string wifi) { ssid = wifi; });
      read(document, "password", [&password] (std::string passwd) { password = passwd; });
      return std::make_tuple(ssid, password);
    }

    const WifiCredentials ConfigReader::getWebCredentials(const rapidjson::Value::ConstObject document)
    {
      using namespace rapidjson;
      std::string web;
      std::string password;
      read(document, "login", [&web] (std::string webLogin) { web = webLogin; });
      read(document, "webpass", [&password] (std::string passwd) { password = passwd; });
      return std::make_tuple(web, password);
    }

    const std::string ConfigReader::getTimeZone(const rapidjson::Value::ConstObject document)
    {
      using namespace rapidjson;
      std::string timeZone = "";
      read(document, "timezone", [&timeZone] (std::optional<std::string> tz) {
        if (tz.has_value())
        {
          timeZone = *tz;
        }
      });
      return std::string(timeZone);
    }

    const mqtt::MQTTConfig ConfigReader::getMQTTConfig(const rapidjson::Value::ConstObject document)
    {
      mqtt::MQTTConfig conf;
      using namespace rapidjson;
      if (!document.HasMember("mqttbroker"))
      {
        return {};
      }
      std::string mqttuser;
      std::string mqttpass;
      std::string ipAddr;
      read(document, "mqttusername", [&mqttuser] (std::string user) { mqttuser = user; });
      read(document, "mqttpasswd", [&mqttpass] (std::string pass) { mqttpass = pass; });
      read(document, "mqttbroker", [&ipAddr] (std::string ip) { ipAddr = ip; });
      conf.credentials = std::make_tuple(mqttuser, mqttpass);
      conf.addr = ipAddr;
      return conf;
    }

    mqtt::MQTTSensorDevice getSensorDevice(const rapidjson::Value::ConstObject device, uint16_t id)
    {
      using namespace mqtt;
      MQTTSensorDevice sensorDevice;
      sensorDevice.deviceId = id;
      MQTTSensorType sensorType = MQTTSensorType::MQTTINVALID;
      read(device, "type", [&](std::string type) { sensorType = mqtt::util::GetSensorType(type); });
      if (sensorType == MQTTSensorType::MQTTINVALID)
      {
        throw std::runtime_error("Unknown Sensor Type detected");
      }
      sensorDevice.sensorType = sensorType;

      sensorDevice.dataType = MQTTSensorDataType::Raw;

      read(device, "jsondata", [&sensorDevice](std::optional<bool> val) {
        if (!val.has_value()) { return; }
        sensorDevice.dataType = *val ? MQTTSensorDataType::JSON : MQTTSensorDataType::Raw;
      });

      if (sensorDevice.dataType == MQTTSensorDataType::JSON)
      {
        std::vector<ValueTuple> vals;
        if (sensorDevice.sensorType == MQTTSensorType::MQTTTwoValues)
        {
          ValueTuple firstValueTuple;
          read(device, MQTTFirstKey.c_str(), [&](std::string temp) {
            firstValueTuple = {MQTTFirstKey, temp, std::string("0")};
          });
          ValueTuple secondValueTuple;
          read(device, MQTTSecondKey.c_str(), [&](std::string humi) {
            secondValueTuple = {MQTTSecondKey, humi, std::string("0")};
          });
          vals.push_back(std::move(firstValueTuple));
          vals.push_back(std::move(secondValueTuple));
        }
        else
        {
          ValueTuple mappedValues;
          read(device, MQTTFirstKey.c_str(), [&](std::string val) {
            mappedValues = {MQTTFirstKey, val, std::string("0")};
          });
          vals.push_back(std::move(mappedValues));
        }
        sensorDevice.mappedValues = vals;
      }
      read(device, "name", [&](std::string name) { sensorDevice.name = name; });
      read(device, "getTopic", [&](std::string topic) { sensorDevice.getTopic = topic; });
      return sensorDevice;
    };

    std::vector<MQTTVariants> getScenes(json::JsonArray scenes)
      {
        using namespace rapidjson;
        using namespace mqtt;
        uint16_t tagId = 0;
        std::vector<MQTTVariants> vecScenes;
        for (const auto& sceneObj : scenes)
        {
          const auto scene = json::convertTo<json::JsonObject>(sceneObj, "scene");
          std::string sceneType;
          read(scene, "type", [&sceneType] (std::string scene) { sceneType = scene; });
          if (sceneType == "Light" || sceneType == "Switch")
          {
            auto aScene = std::make_shared<MQTTSwitchGroup>();
            read(scene, "name", [&aScene] (std::string name) { aScene->sceneName = name; });
            read(scene, "icon", [&aScene] (std::string icon) { aScene->iconName = icon; });
            GroupDevices<MQTTSwitchDevice> devices;
            int deviceId = 0;
            read(scene, "devices", [&] (json::JsonArray jsonDevices) {
              for (const auto& jsonDevice : jsonDevices)
              {
                const auto device = json::convertTo<json::JsonObject>(jsonDevice, "device");
                MQTTSwitchDevice switchDevice;
                switchDevice.deviceId = deviceId;
                read(device, "name", [&switchDevice] (std::string name) { switchDevice.name = name; });
                read(device, "getTopic", [&switchDevice] (std::string topic) { switchDevice.getTopic = topic; });
                read(device, "setTopic", [&switchDevice] (std::string topic) { switchDevice.setTopic = topic; });
                read(device, "onValue", [&switchDevice] (std::string value) { switchDevice.onValue = value; });
                read(device, "offValue", [&switchDevice] (std::string value) { switchDevice.offValue = value; });
                devices.push_back({switchDevice.getTopic, std::move(switchDevice)});
                deviceId += 1;
              }
              aScene->mDevices = devices;
              aScene->groupId = tagId;
              MQTTVariants variant = aScene;
              vecScenes.push_back(aScene);
            });
          }
          else if (sceneType == "Sensor")
          {
            auto aScene = std::make_shared<MQTTSensorGroup>();
            read(scene, "name", [&aScene] (std::string name) { aScene->sceneName = name; });
            read(scene, "icon", [&aScene] (std::string icon) { aScene->iconName = icon; });
            GroupDevices<MQTTSensorDevice> devices;
            int deviceId = 0;
            read(scene, "devices", [&] (json::JsonArray jsonDevices) {
              for (const auto& device : jsonDevices)
              {
                const auto deviceObj = json::convertTo<json::JsonObject>(device, "device");
                MQTTSensorDevice sensorDevice = getSensorDevice(deviceObj, deviceId);
                devices.push_back({sensorDevice.getTopic, std::move(sensorDevice)});
                deviceId += 1;
              }
            });
            
            aScene->mSensorDevices = devices;
            aScene->groupId = tagId;
            MQTTVariants variant = aScene;
            vecScenes.push_back(aScene);
          }
          tagId += 1;
        }
        return vecScenes;
      }
    
    const std::vector<MQTTVariants> ConfigReader::getMQTTGroups(const rapidjson::Value::ConstObject document)
    {
      using namespace rapidjson;
      using namespace mqtt;
      if (!document.HasMember("scenes"))
      {
        return {};
      }
      std::vector<MQTTVariants> vecScenes;
      read(document, "scenes", [&] (json::JsonArray array) { vecScenes = getScenes(array); });
      return vecScenes;
    };

  
} // namespace fs
