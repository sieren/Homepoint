#pragma once
#include <AppNewScreen.h>
#include <config/Icon.hpp>
#include <mqtt/MQTTSensorGroup.hpp>
#include <mqtt/MQTTSensorTypes.hpp>
#include <ui/UIMosaicButton.h>
#include <ui/UISensorComboButton.h>

#include <lvgl.h>

#include <vector>

namespace gfx
{
namespace util
{
  template<typename ScreenType>
  struct UISceneButtonBuilder
  {
    auto operator()(MQTTSwitchGroupPtr ptr, ScreenType screen, lv_obj_t* pParent)
      -> std::shared_ptr<UIMosaicButton>
    {
      auto button = std::make_shared<UIMosaicButton>(pParent, ptr->sceneName);
      const auto groupId = ptr->groupId;
      button->setupButton();
      button->setTextLabel(ptr->sceneName);

      const auto imageName = ptr->iconName;
      const auto state = ptr->currentState();
      // Switch Textlabel color based on if some or all devices are on.
      button->setImage(imageName);
      button->setState(state);
      const auto ctx = screen->mpAppContext;
      button->addTargetLongPressAction([ptr, ctx](const uint16_t id) {
        const bool isActive =  ptr->isActive();
        ctx->getMQTTConnection()->switchScene(id, !isActive);
      });
      button->addTargetAction([screen, groupId](const uint16_t id)
      {
        screen->presentTilesForScene(groupId);
      });
  
      ptr->mSetNeedsUpdateCB = [ptr, weakBtn = std::weak_ptr<UIMosaicButton>(button)]() {
        using namespace mqtt;
        const auto state = ptr->currentState();
        auto button = weakBtn.lock();
        if (!button)
        {
          return;
        }
        button->setState(state);
      };
      return button;
    };

    auto operator()(MQTTSensorGroupPtr ptr, ScreenType screen, lv_obj_t* pParent) -> std::shared_ptr<UIMosaicButton>
    {
      const bool hasMultipleSensors = isMultiSensor(ptr);
      auto button = std::make_shared<UISensorComboButton>(pParent, ptr->sceneName, ptr->groupId, hasMultipleSensors);
      button->setupButton();
      button->setTextLabel(ptr->sceneName);
      setMultiSensorStates(ptr, button);
      ptr->mSetNeedsUpdateCB = [ptr, weakBtn = std::weak_ptr<UISensorComboButton>(button)]() {
        auto button = weakBtn.lock();
        if (!button)
        {
          return;
        }

        setMultiSensorStates(ptr, button);
      };
      return button;
    };

    std::shared_ptr<UIMosaicButton> operator()(auto ptr, ScreenType screen, lv_obj_t* pParent)
    {
      // NO OP, means unimplemented handler
      // TODO: Abort or crash instead
      std::shared_ptr<UIMosaicButton> button = std::make_shared<UIMosaicButton>(pParent, "");
      return button;
    };

    static bool isMultiSensor(auto ptr)
    {
      using namespace mqtt;
      if (ptr->mSensorDevices.size() > 1) { return true; }
      for (auto& sensor : ptr->mSensorDevices)
      {
        switch(sensor.second.sensorType)
        {
          case MQTTSensorType::MQTTCombinedJSONValues:
          {
            return true;
            break;
          }

          case MQTTSensorType::MQTTSingleValue:
          {
            return false;
            break;
          }

          default:
            return false;
            break;
        }
      }
      return false;
    };

    // Helper
    static void setMultiSensorStates(auto ptr, auto button)
    {
      using namespace mqtt::util;
      using namespace mqtt;
      button->eraseValues();
      std::vector<std::pair<UISensorComboButton::ImagePath, UISensorComboButton::ValueType>> valuePairs;
      for (auto& sensor : ptr->mSensorDevices)
      {
        switch(sensor.second.sensorType)
        {
          case MQTTSensorType::MQTTCombinedJSONValues:
          {
            const auto firstValue = sensor.second.getFirstValue();
            const auto secondValue = sensor.second.getSecondValue();

            const auto firstValueIcon = sensor.second.firstIconName;
            const auto secondValueIcon = sensor.second.secondIconName.has_value() ?
              *sensor.second.secondIconName : firstValueIcon; // fallback
            valuePairs.push_back({firstValueIcon, firstValue});
            valuePairs.push_back({secondValueIcon, secondValue});
            break;
          }

          case MQTTSensorType::MQTTSingleValue:
          {
            const auto firstValue = sensor.second.getFirstValue();
            const auto firstValueIcon = sensor.second.firstIconName;
            valuePairs.push_back({firstValueIcon, firstValue});
            break;
          }

          default:
            break;
        }
      }
      button->updateImageWithValue(valuePairs);
    }
  };
} // namespace util
} // namespace gfx