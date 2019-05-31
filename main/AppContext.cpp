#include "AppContext.h"
#include "rapidjson/stringbuffer.h"

#include <memory>

namespace ctx
{
  AppContext::AppContext()
    {
    };

  void AppContext::setup()
  {
    fs::FileSystem::getInstance().loadPartitions();
    mDeviceGroups = fs::ConfigReader::getDeviceGroups();
    const auto hkConfig = fs::ConfigReader::getHKConfig();
    if (hkConfig.isEnabled)
    {
      mpHKConnection = std::make_shared<homekit::HKConnection>(hkConfig, mDeviceGroups);
    }
    mpMQTTConnection = std::make_shared<mqtt::MQTTConnection>(fs::ConfigReader::getMQTTConfig(), mDeviceGroups);
    const auto timeZone = fs::ConfigReader::getTimeZone();
    if (timeZone != "")
    {
      mNTPSync = std::make_shared<ntp::NTPSync>(timeZone);
      mWifiContext.registerCallback([ntpSync = std::weak_ptr<ntp::NTPSync>(mNTPSync)](const auto& connState)
      {
        if (connState.wifiState == ctx::WifiAssociationState::CONNECTED)
        {
          auto ntpClient = ntpSync.lock();
          if (ntpClient)
          {
            ntpClient->syncTime();
          }
        }
      });
    }
    mpMQTTConnection->registerConnectionStatusCallback([&](auto cb) {
      if (cb == mqtt::MQTTConnectionStatus::CONNECTED)
      {
        mpMQTTConnection->bindScenes();
        if (mpHKConnection)
        {
          mpHKConnection->start();
        }
      }
    });
  }

  std::vector<DeviceVariants> &AppContext::getDeviceGroups()
  {
    return mDeviceGroups;
  }
} // namespace ctx
