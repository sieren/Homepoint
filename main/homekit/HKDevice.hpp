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

    static void switch_on_callback(homekit_characteristic_t *_ch, homekit_value_t on, void *context)
    {
        printf("Switch On Callback\r\n");
        printf("With ID: %i\r\n", _ch->id);
        if (_ch->value.bool_value != on.bool_value)
        {
          _ch->value.bool_value = on.bool_value;
          homekit_characteristic_notify(_ch, _ch->value);
        }
        (*static_cast<HKDevice*>(context)).setDeviceState(on.bool_value);
    }

    static void deviceSetCallback(homekit_characteristic_t *ch, const homekit_value_t value)
    {
      (*static_cast<HKDevice*>(ch->context)).setDeviceState(value.bool_value);
      printf("Device has been set!");
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
      printf("Updated with new value");
      printf(sceneName.c_str());
      printf("\r\n");
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