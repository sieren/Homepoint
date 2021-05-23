#include "AppContext.h"
#include "rapidjson/stringbuffer.h"

#include <memory>

namespace ctx
{
  void AppContext::setup()
  {
    fs::FileSystem::getInstance().loadPartitions();
    mpWifiContext = std::make_shared<WifiContext>();
    try
    {
      mModel = fs::ConfigReader().readConfiguration();
    }
    catch(const std::exception& e)
    {
      mModel = fs::ConfigReader().readFailsafeConfiguration();
      connectWireless();
      mpWebServer = std::make_unique<web::WebServer>(shared_from_this(), mModel.mWebCredentials);
      throw std::runtime_error("Configuration invalid! Login via browser" + std::string(e.what()));
    }
    if (!mModel.hasWifiCredentials())
    {
      mpCaptiveServer = std::make_unique<wifi::CaptiveServer>(shared_from_this());
      return;
    }
    mpMQTTConnection = std::make_shared<mqtt::MQTTConnection>(mModel.mMQTTServerConfig, mModel.mMQTTGroups);
    connectWireless();
    mpMQTTConnection->registerConnectionStatusCallback([&](auto cb) {
      if (cb == mqtt::MQTTConnectionStatus::CONNECTED)
      {
        mpMQTTConnection->bindScenes();
        mAppStateNotifier.broadcast(ContextState::Ready);
      }
    });
    mpWebServer = std::make_unique<web::WebServer>(shared_from_this(), mModel.mWebCredentials);
    mAppStateNotifier.broadcast(ContextState::Ready);
  }

  void AppContext::reload()
  {
    mAppStateNotifier.broadcast(ContextState::Reload);
    try
    {
      mModel = fs::ConfigReader().readConfiguration();
    }
    catch(const std::exception& e)
    {
      // Reboot to failsafe
      ESP.restart();
    }
    mpMQTTConnection = std::make_shared<mqtt::MQTTConnection>(mModel.mMQTTServerConfig, mModel.mMQTTGroups);
    mpMQTTConnection->registerConnectionStatusCallback([&](auto cb) {
      if (cb == mqtt::MQTTConnectionStatus::CONNECTED)
      {
        mpMQTTConnection->bindScenes();
        mAppStateNotifier.broadcast(ContextState::Ready);
      }
      Serial.println("Connected");
    });
    mpMQTTConnection->connect();
  }

  void AppContext::setFirstLaunch(const WifiCredentials credentials,
    const std::string login, const std::string username)
  {
    fs::ConfigReader().setFirstLaunch(credentials, login, username);
    ESP.restart();
  }

  Dispatcher<ctx::ContextState>::CBID AppContext::registerStateCallback(AppStateCB callback)
  {
    return mAppStateNotifier.addCB(callback);
  }

  void AppContext::deleteStateCallback(Dispatcher<ctx::ContextState>::CBID callback)
  {
    mAppStateNotifier.delCB(callback);
  }

  void AppContext::connectWireless()
  {
    auto& wifi = mModel.mWifiCredentials;
    mpWifiContext->connect(wifi.mSSID, wifi.mPassword, wifi.mHostname);
    using namespace std::placeholders;
    mpWifiContext->registerCallback(std::bind(&AppContext::connectionStateChanged, this, _1));
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
        mNTPSync = std::make_shared<ntp::NTPSync>(mModel.mTimeZone);
        mNTPSync->syncTime();
      }
      if (mpMQTTConnection && !mModel.mMQTTServerConfig.addr.empty())
      {
        mpMQTTConnection->connect();
      }
      mpWebServer->startServer();
    }
  }
} // namespace ctx
