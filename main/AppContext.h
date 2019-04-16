#pragma once

#include <mqtt/MQTTConnection.h>
#include <mqtt/MQTTGroup.hpp>
#include <ntp/NTPSync.h>
#include <wifi/WifiContext.h>
#include "rapidjson/document.h"
#include <memory>
#include <tuple>
#include <string>
#include <vector>


namespace ctx
{
  class AppContext
  {
    public:
      AppContext();
      void setup();

      WifiContext& getWifiContext() { return mWifiContext; };
      std::shared_ptr<mqtt::MQTTConnection> getMQTTConnection() { return mpMQTTConnection; };
      std::vector<MQTTVariants> &getMQTTGroups();

    private: 
      std::shared_ptr<mqtt::MQTTConnection> mpMQTTConnection;
      std::shared_ptr<ntp::NTPSync> mNTPSync;
      WifiContext mWifiContext;
      std::vector<MQTTVariants> mMQTTGroups;
      rapidjson::Document mConfigDocument;
  };
} // namespace ctx
