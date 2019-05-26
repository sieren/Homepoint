#pragma once
#include <AppScreen.hpp>
#include <config/Icon.hpp>
#include <mqtt/MQTTSensorGroup.hpp>
#include <ui/UIButton.h>
#include <ui/UISensorComboWidget.h>
#include <ui/UIWidget.hpp>

#include <string>

namespace gfx
{
namespace util
{
  template<typename ScreenType>
  struct UIDetailButtonBuilder
  {
    auto operator()(MQTTSwitchGroupPtr ptr, ScreenType screen) -> std::vector<std::shared_ptr<UIWidget>>
    {
      std::vector<std::shared_ptr<UIWidget>> buttons;
      auto& context = screen->mpAppContext;
      for (auto& sensor : ptr->mDevices)
      {
        auto button = std::make_shared<UIButton>(&(screen->mTft), Frame(), ptr->groupId);
        button->setBackgroundColor(Color::InactiveBgColor());
        // TODO use Icons for Devices
        const auto icons = GetIconFileNames(ptr->iconName);

        button->setLabel(sensor.second.name);
        const auto textColor = sensor.second.active ? Color::ActiveBgColor() : Color::InactiveTextColor();
        const auto imagePath = sensor.second.active ? icons.first : icons.second;
        button->setImage(imagePath);
        button->setTextColor(textColor);
        auto groupId = ptr->groupId;
        auto deviceId = sensor.second.deviceId;
        button->addTargetAction([groupId, deviceId, &sensor, context](const uint16_t id) {
          const bool isActive = sensor.second.active;
          context->getMQTTConnection()->switchDevice(groupId, deviceId, !isActive);
        });

        sensor.second.mSetNeedsUpdateCB = [ptr, deviceId, weakBtn = std::weak_ptr<UIButton>(button), icons]() {
          auto findSensor = std::find_if(ptr->mDevices.begin(), ptr->mDevices.end(), [&deviceId](auto& ele)
          {
            return ele.second.deviceId == deviceId;
          });
          if (findSensor == ptr->mDevices.end())
          {
            return;
          }
          const auto textColor = (*findSensor).second.active ? Color::ActiveBgColor() : Color::InactiveTextColor();
          const auto imagePath = (*findSensor).second.active ? icons.first : icons.second;
          auto button = weakBtn.lock();

          if (!button)
          {
            return;
          }

          button->setTextColor(textColor);
          button->setImage(imagePath);
        };
        buttons.push_back(button);
      }
      return buttons;
    }

    std::vector<std::shared_ptr<UIWidget>> operator()(auto& ptr, ScreenType screen)
    {
      // IF THIS IS HIT, A HANDLER IS MISSING.
      std::vector<std::shared_ptr<UIWidget>> buttons;
      return buttons;
    }

  };
} // namespace util
} // namespace gfx