#include "AppContext.h"
#include "rapidjson/stringbuffer.h"

#include <memory>

namespace ctx
{
  void AppContext::setup()
  {
    fs::FileSystem::getInstance().loadPartitions();
    mModel = fs::ConfigReader().readConfiguration();
    mpMQTTConnection = std::make_shared<mqtt::MQTTConnection>(mModel.mMQTTServerConfig, mModel.mMQTTGroups);
    getWifiContext().connect(std::get<0>(mModel.mWifiCredentials), std::get<1>(mModel.mWifiCredentials));
    if (mModel.mTimeZone != "")
    {
      mNTPSync = std::make_shared<ntp::NTPSync>(mModel.mTimeZone);
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
    return mModel.mMQTTGroups;
  }
} // namespace ctx
