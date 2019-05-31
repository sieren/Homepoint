#pragma once

#include <devices/DeviceTypes.hpp>
#include "HKConfig.hpp"
#include "HKDevice.hpp"
#include <vector>

namespace homekit
{
  class HKConnection
  {
    public:
      HKConnection() = delete;
      HKConnection(const HKConfig& config, const std::vector<DeviceVariants> devices);
      void start();

    private:
      std::vector<DeviceVariants> mDevices;
      const HKConfig mConfig;
  };
}