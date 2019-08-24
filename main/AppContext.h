#pragma once

#include <homekit/HKConnection.h>
#include <mqtt/MQTTConnection.h>
#include <mqtt/MQTTGroup.hpp>
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
      AppContext();
      void setup();

      WifiContext& getWifiContext() { return mWifiContext; };
      std::shared_ptr<mqtt::MQTTConnection> getMQTTConnection() { return mpMQTTConnection; };
      std::vector<DeviceVariants> &getDeviceGroups();

    private: 
      std::shared_ptr<mqtt::MQTTConnection> mpMQTTConnection;
      std::shared_ptr<homekit::HKConnection> mpHKConnection = nullptr;
      std::shared_ptr<ntp::NTPSync> mNTPSync;
      WifiContext mWifiContext;
      std::vector<DeviceVariants> mDeviceGroups;
      rapidjson::Document mConfigDocument;
  };
} // namespace ctx
