#pragma once

#include <model/Model.hpp>
#include <ntp/NTPSync.h>
#include <util/dispatcher.hpp>
#include <wifi/CaptiveServer.h>
#include <wifi/WifiContext.h>
#include <web/WebServer.h>
#include <fs/ConfigReader.hpp>
#include <memory>
#include <tuple>
#include <string>
#include <vector>


namespace ctx
{
  enum class ContextState: int {
    Reload,
    Ready
  };

  using AppStateCB = std::function<void(ctx::ContextState)>;

  class AppContext : public std::enable_shared_from_this<AppContext>
  {
    public:
      AppContext() = default;
      void setup();

      WifiContext& getWifiContext() { return mWifiContext; };
      model::Model& getModel() { return mModel; }
      std::shared_ptr<mqtt::MQTTConnection> getMQTTConnection() { return mpMQTTConnection; };
      std::vector<MQTTVariants> &getMQTTGroups();
      void reload();
      void connectionStateChanged(ctx::WifiConnectionState state);
      void setFirstLaunch(const WifiCredentials credentials,
        const std::string login, const std::string username);
      void registerStateCallback(AppStateCB callback);

    private: 
      void connectWireless();
      Dispatcher<ContextState> mAppStateNotifier;
      std::shared_ptr<mqtt::MQTTConnection> mpMQTTConnection;
      std::shared_ptr<ntp::NTPSync> mNTPSync;
      std::unique_ptr<wifi::CaptiveServer> mpCaptiveServer;
      std::unique_ptr<web::WebServer> mpWebServer;
      WifiContext mWifiContext;
      rapidjson::Document mConfigDocument;
      model::Model mModel;
  };
} // namespace ctx
