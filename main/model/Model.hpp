#pragma once

#include <config/Config.h>
#include <mqtt/MQTTConnection.h>
#include <mqtt/MQTTGroup.hpp>
#include <mqtt/MQTTSensorGroup.hpp>

#include <map>
#include <string>
#include <tuple>
#include <vector>

struct WifiCredentials
{
  std::string mSSID;
  std::string mPassword;
  std::optional<std::string> mHostname;
};
using WebCredentials = std::tuple<std::string, std::string>;

namespace model
{
struct Model
{
  WifiCredentials mWifiCredentials;
  WebCredentials mWebCredentials;
  std::optional<std::string> mHostName;
  config::HardwareConfig mHardwareConfig;
  std::string mTimeZone;
  mqtt::MQTTConfig mMQTTServerConfig;
  std::vector<MQTTVariants> mMQTTGroups;

  bool hasWifiCredentials()
  {
    return mWifiCredentials.mSSID != " ";
  }
};
} // namespace model
