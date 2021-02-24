#pragma once
#include <AppNewScreen.h>
#include <config/Icon.hpp>
#include <mqtt/MQTTSensorGroup.hpp>
#include <ui/UISwitchDeviceButton.h>
#include <ui/UIMosaicButton.h>
// #include <ui/UISensorComboWidget.h>

#include <string>

namespace gfx
{
namespace util
{
  template<typename ScreenType>
  struct UIDetailButtonBuilder
  {
    auto operator()(auto& switchDevice, MQTTSwitchGroupPtr ptr, ScreenType screen, lv_obj_t* pParent) -> MosaicButtonPtr
    {
      auto& context = screen->mpAppContext;
      const auto label = switchDevice.second.name;
      auto button = std::make_shared<UISwitchDeviceButton>(pParent, label, ptr->groupId);
      button->setupButton();
      // button->setBackgroundColor(Color::InactiveBgColor());

      // // Check if device has icon, else fallback to group icon
      const bool hasIcon = switchDevice.second.iconName.has_value();
      const auto icon = hasIcon ? *switchDevice.second.iconName : ptr->iconName;
      button->setTextLabel(switchDevice.second.name);
      const auto isActive = switchDevice.second.active;
      const auto imagePath = icon;
      button->setImage(imagePath);
      button->setState(isActive);
      auto groupId = ptr->groupId;
      auto deviceId = switchDevice.second.deviceId;
      button->addTargetAction([groupId, deviceId, &switchDevice, context](const uint16_t id) {
        const bool isActive = switchDevice.second.active;
        context->getMQTTConnection()->switchDevice(groupId, deviceId, !isActive);
      });
      button->addTargetLongPressAction([screen](const uint16_t id)
      {
      //  screen->presentTilesForScene(id);
      });

      switchDevice.second.mSetNeedsUpdateCB = [ptr, deviceId, weakBtn = std::weak_ptr<UISwitchDeviceButton>(button)]()
      {
        auto findSensor = std::find_if(ptr->mDevices.begin(), ptr->mDevices.end(), [&deviceId](auto& ele)
        {
          return ele.second.deviceId == deviceId;
        });
        if (findSensor == ptr->mDevices.end())
        {
          return;
        }
        const auto isActive = (*findSensor).second.active;
      //   const auto imagePath = (*findSensor).second.active ? icons.first : icons.second;
        auto button = weakBtn.lock();
        if (!button)
        {
          return;
        }

        button->setState(isActive);
      };
      return button;
    }

    auto operator()(auto& switchDevice, auto ptr, ScreenType screen, lv_obj_t* pParent)
    {
      // IF THIS IS HIT, A HANDLER IS MISSING.
      MosaicButtonPtr button;
      return button;
    }

  };
} // namespace util
} // namespace gfx
