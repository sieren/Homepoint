#include "AppContext.h"
#include <fs/ConfigReader.hpp>
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
    mMQTTGroups = fs::ConfigReader::getMQTTGroups();
    mpMQTTConnection = std::make_shared<mqtt::MQTTConnection>(fs::ConfigReader::getMQTTConfig(), mMQTTGroups);
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
      }
    });
  }

  std::vector<MQTTVariants> &AppContext::getMQTTGroups()
  {
    return mMQTTGroups;
  }
} // namespace ctx
