#pragma once
#include <AppScreen.hpp>
#include <config/Icon.hpp>
#include <mqtt/MQTTSensorGroup.hpp>
#include <ui/UIButton.h>
#include <ui/UISensorComboWidget.h>
#include <ui/UIWidget.hpp>

namespace gfx
{
namespace util
{
  template<typename ScreenType>
  struct UISceneButtonBuilder
  {
    auto operator()(MQTTSwitchGroupPtr ptr, ScreenType screen) -> std::shared_ptr<UIButton>
    {
      auto button = std::make_shared<UIButton>(&(screen->mTft), Frame(), ptr->groupId);
      button->setBackgroundColor(Color::InactiveBgColor());
      button->setLabel(ptr->sceneName);

      const auto icons = GetIconFileNames(ptr->iconName);
      const auto textColor = ptr->isActive() ? Color::ActiveBgColor() : Color::InactiveTextColor();
      const auto imagePath = ptr->isActive() ? icons.first : icons.second;
      button->setImage(imagePath);
      button->setTextColor(textColor);
      auto& context = screen->mpAppContext;
      button->addTargetAction([ptr, context](const uint16_t id) {
        const bool isActive =  ptr->isActive();
        context->getMQTTConnection()->switchScene(id, !isActive);
      });
      button->addTargetLongPressAction([screen](const uint16_t id)
      {
        screen->presentScreen(id);
      });
      auto& screenSaver = screen->mScreenSaver; 
      ptr->mSetNeedsUpdateCB = [ptr, weakBtn = std::weak_ptr<UIButton>(button), &screenSaver, icons]() {
        const auto textColor = ptr->isActive() ? Color::ActiveBgColor() : Color::InactiveTextColor();
        const auto imagePath = ptr->isActive() ? icons.first : icons.second;
        auto button = weakBtn.lock();
        if (!button)
        {
          return;
        }
        button->setTextColor(textColor);
        button->setImage(imagePath);
        screenSaver.activate();
      };
      return button;
    };

    auto operator()(MQTTSensorGroupPtr ptr, ScreenType screen) -> std::shared_ptr<UIWidget>
    {
      auto button = std::make_shared<UISensorComboWidget>(&(screen->mTft), Frame(), ptr->groupId);
      button->setLabel(ptr->sceneName);
      button->setTextColor(Color::WhiteColor());
      setMultiSensorStates(ptr, button);
      ptr->mSetNeedsUpdateCB = [ptr, weakBtn = std::weak_ptr<UISensorComboWidget>(button)]() {
        const auto textColor = Color::InactiveTextColor();
        auto button = weakBtn.lock();
        if (!button)
        {
          return;
        }
        button->setTextColor(textColor);

        setMultiSensorStates(ptr, button);
      };
      return button;
    };

    std::shared_ptr<UIButton> operator()(auto ptr, ScreenType screen)
    {
      // NO OP, means unimplemented handler
      // TODO: Abort or crash instead
      std::shared_ptr<UIButton> button = std::make_shared<UIButton>(&(screen->mTft), Frame(), 999999);
      return button;
    };

    // Helper

    static void setMultiSensorStates(auto ptr, auto button)
    {
      using namespace mqtt::util;
      using namespace mqtt;
      button->eraseValues();
      for (auto& sensor : ptr->mSensorDevices)
      {
        switch(sensor.second.sensorType)
        {
          case MQTTSensorType::MQTTTemperatureHumidity:
          {
            const auto temperature = sensor.second.getTemperature();
            const auto humidity = sensor.second.getHumidity();
            const auto humidityIcon = util::GetSmallIcon(util::humidityIcon);
            const auto temperatureIcon = util::GetSmallIcon(util::temperatureIcon);
            button->setImageWithValue({temperatureIcon, temperature});
            button->setImageWithValue({humidityIcon, humidity});
            break;
          }
          case MQTTSensorType::MQTTTemperature:
          {
            const auto temperature = sensor.second.getTemperature();
            const auto temperatureIcon = util::GetSmallIcon(util::temperatureIcon);
            button->setImageWithValue({temperatureIcon, temperature});
            break;
          }
          case MQTTSensorType::MQTTHumidity:
          {
            const auto humidityIcon = util::GetSmallIcon(util::humidityIcon);
            const auto humidity = sensor.second.getHumidity();
            button->setImageWithValue({humidityIcon, humidity});
            break;
          }

          case MQTTSensorType::MQTTVOC:
          {
            const auto vocIcon = util::GetSmallIcon(util::vocIcon);
            const auto vocVal = sensor.second.getVOC();
            button->setImageWithValue({vocIcon, vocVal});
            break;
          }
          default:
            break;
        }
      }
    }
  };
} // namespace util
} // namespace gfx