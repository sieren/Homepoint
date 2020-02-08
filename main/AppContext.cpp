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
    using namespace std::placeholders;
    getWifiContext().registerCallback(std::bind(&AppContext::connectionStateChanged, this, _1));
    mNTPSync = mModel.mTimeZone != "" ? std::make_shared<ntp::NTPSync>(mModel.mTimeZone) : nullptr;
    mpMQTTConnection->registerConnectionStatusCallback([&](auto cb) {
      if (cb == mqtt::MQTTConnectionStatus::CONNECTED)
      {
        mpMQTTConnection->bindScenes();
      }
    });
    mpWebServer = std::make_unique<web::WebServer>(shared_from_this());
  }

  std::vector<MQTTVariants> &AppContext::getMQTTGroups()
  {
    return mModel.mMQTTGroups;
  }

  void AppContext::connectionStateChanged(ctx::WifiConnectionState state)
  {
    if (state.wifiState == ctx::WifiAssociationState::CONNECTED)
    {
      if (mModel.mTimeZone != "")
      {
        mNTPSync->syncTime();
      }
      mpMQTTConnection->connect();
      mpWebServer->startServer();
    }
  }
} // namespace ctx
