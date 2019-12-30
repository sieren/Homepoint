#pragma once

#include <model/Model.hpp>
#include <ntp/NTPSync.h>
#include <wifi/WifiContext.h>
#include <fs/ConfigReader.hpp>
#include <memory>
#include <tuple>
#include <string>
#include <vector>


namespace ctx
{
  class AppContext
  {
    public:
      AppContext() = default;
      void setup();

      WifiContext& getWifiContext() { return mWifiContext; };
      std::shared_ptr<mqtt::MQTTConnection> getMQTTConnection() { return mpMQTTConnection; };
      std::vector<MQTTVariants> &getMQTTGroups();
      void connectionStateChanged(ctx::WifiConnectionState state);

    private: 
      std::shared_ptr<mqtt::MQTTConnection> mpMQTTConnection;
      std::shared_ptr<ntp::NTPSync> mNTPSync;
      WifiContext mWifiContext;
      rapidjson::Document mConfigDocument;
      model::Model mModel;
  };
} // namespace ctx
