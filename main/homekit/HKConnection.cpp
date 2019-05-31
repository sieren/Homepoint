#include <devices/DeviceTypes.hpp>
#include <util/varianthelper.hpp>
#include "HKConnection.h"
#include "HKDevice.hpp"

extern "C"
{
  #include <cstdio>
  #include <cstddef>
  #include <homekit/types.h>
  #include "hkapi.h"
}

namespace homekit
{

  HKConnection::HKConnection(const HKConfig& config, const std::vector<DeviceVariants> devices) :
    mDevices(devices),
    mConfig(config)
    {
      hkInitService();
      for (auto& device : mDevices)
      {
        mpark::visit(::util::overloaded(
          [&](HKDevicePtr& ptr)
        { 
          hkAddDevice(ptr->sceneName.c_str(), ptr->characteristic);
        },
        [](auto&& i) {}), device);
      }
    }

  void HKConnection::start()
  {
    hkInit(mConfig.password.c_str());
  }
} // namespace homekit
