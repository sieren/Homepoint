#pragma once

#include <config/Config.h>
#include <mqtt/MQTTConnection.h>
#include <mqtt/MQTTGroup.hpp>
#include <mqtt/MQTTSensorGroup.hpp>

#include <map>
#include <string>
#include <tuple>
#include <vector>

using WifiCredentials = std::tuple<std::string, std::string>;
using WebCredentials = std::tuple<std::string, std::string>;

namespace model
{
struct Model
{
  WifiCredentials mWifiCredentials;
  WebCredentials mWebCredentials;
  config::HardwareConfig mHardwareConfig;
  std::string mTimeZone;
  mqtt::MQTTConfig mMQTTServerConfig;
  std::vector<MQTTVariants> mMQTTGroups;

  bool hasWifiCredentials()
  {
    return !(std::get<0>(mWifiCredentials) == " " &&
      std::get<0>(mWifiCredentials) == " ");
  }
};
} // namespace model
