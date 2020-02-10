#include "AppContext.h"
#include "rapidjson/stringbuffer.h"

#include <memory>

namespace ctx
{
  void AppContext::setup()
  {
    fs::FileSystem::getInstance().loadPartitions();
    try
    {
      mModel = fs::ConfigReader().readConfiguration();
    }
    catch(const std::exception& e)
    {
      mModel = fs::ConfigReader().readFailsafeConfiguration();
      mpWebServer = std::make_unique<web::WebServer>(shared_from_this(), mModel.mWebCredentials);
      throw std::runtime_error("Configuration invalid! Login via browser");
    }
    if (!mModel.hasWifiCredentials())
    {
      mpCaptiveServer = std::make_unique<wifi::CaptiveServer>(shared_from_this());
      return;
    }
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
    mpWebServer = std::make_unique<web::WebServer>(shared_from_this(), mModel.mWebCredentials);
  }

  void AppContext::setFirstLaunch(const WifiCredentials credentials,
    const std::string login, const std::string username)
  {
    fs::ConfigReader().setFirstLaunch(credentials, login, username);
    ESP.restart();
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
