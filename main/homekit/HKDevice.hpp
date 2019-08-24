#pragma once

#include <memory>
#include <string>

extern "C"
{
  #include <cstdio>
  #include <cstddef>
  #include <homekit/characteristics.h>
  #include <homekit/types.h>
}
namespace homekit
{
  struct HKDevice;
} // namespace homekit

using HKDevicePtr = std::shared_ptr<homekit::HKDevice>;
namespace homekit
{
  struct HKDevice
  {
    static void deviceSetCallback(homekit_characteristic_t *ch, const homekit_value_t value)
    {
      (*static_cast<HKDevice*>(ch->context)).setDeviceState(value.bool_value);
    }

    void setDeviceState(bool on)
    {
      if (on != mIsActive)
      {
        homekit_value_t newValue;
        newValue.format = homekit_format_bool;
        newValue.bool_value = on;
        characteristic->value = newValue;
        mIsActive = on;
        homekit_characteristic_notify(characteristic, characteristic->value);
        if (mSetNeedsUpdateCB)
        {
          mSetNeedsUpdateCB(on);
        }
      }
    }

    HKDevice(const HKDevice&) =delete;
    HKDevice& operator=(const HKDevice&) =delete;

    HKDevice& operator=(HKDevice&& other)
    {
      characteristic = other.characteristic;
      sceneName = other.sceneName;
      characteristic->context = this;
      return *this;
    }

    HKDevice(const HKDevice&& other)
    {
      characteristic = other.characteristic;
      sceneName = other.sceneName;
      characteristic->context = this;
    }

    HKDevice()
    {
      characteristic = new homekit_characteristic_t{};
      characteristic->setter_ex = deviceSetCallback;
      characteristic->context = this;
      characteristic->type = HOMEKIT_CHARACTERISTIC_ON;
      characteristic->description = "On";
      characteristic->format = homekit_format_bool;
      characteristic->permissions = static_cast<homekit_permissions_t>(homekit_permissions_paired_read |
        homekit_permissions_paired_write | homekit_permissions_notify);
      homekit_value_t val;
      val.bool_value = true;
      characteristic->value = val;
    }

    bool isActive() const
    {
      return mIsActive;
    }

    std::string sceneName = "";
    std::string iconName = "";
    uint16_t groupId;
    bool mIsActive = false;
    std::function<void(bool)> mSetNeedsUpdateCB;
    homekit_characteristic_t* characteristic;
  };
} // namespace homekit