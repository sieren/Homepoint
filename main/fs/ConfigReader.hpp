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
    const model::Model readConfiguration();

    const model::Model readFailsafeConfiguration();

    void setFirstLaunch(const WifiCredentials credentials,
      std::string login, std::string password);

  private:
    const WifiCredentials getWifiCredentials(const rapidjson::Value::ConstObject document);
    const WifiCredentials getWebCredentials(const rapidjson::Value::ConstObject document);
    const std::string getTimeZone(const rapidjson::Value::ConstObject document);
    const mqtt::MQTTConfig getMQTTConfig(const rapidjson::Value::ConstObject document);
    const std::vector<MQTTVariants> getMQTTGroups(const rapidjson::Value::ConstObject document);
    const config::HardwareConfig getHardwareConfig(const rapidjson::Value::ConstObject document);

};
} // namespace fs
