#pragma once

#include <model/Model.hpp>
#include <ntp/NTPSync.h>
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
  class AppContext : public std::enable_shared_from_this<AppContext>
  {
    public:
      AppContext() = default;
      void setup();

      WifiContext& getWifiContext() { return mWifiContext; };
      model::Model& getModel() { return mModel; }
      std::shared_ptr<mqtt::MQTTConnection> getMQTTConnection() { return mpMQTTConnection; };
      std::vector<MQTTVariants> &getMQTTGroups();
      void connectionStateChanged(ctx::WifiConnectionState state);
      void setFirstLaunch(const WifiCredentials credentials,
        const std::string login, const std::string username);

    private: 
      void connectWireless();
      std::shared_ptr<mqtt::MQTTConnection> mpMQTTConnection;
      std::shared_ptr<ntp::NTPSync> mNTPSync;
      std::unique_ptr<wifi::CaptiveServer> mpCaptiveServer;
      std::unique_ptr<web::WebServer> mpWebServer;
      WifiContext mWifiContext;
      rapidjson::Document mConfigDocument;
      model::Model mModel;
  };
} // namespace ctx
