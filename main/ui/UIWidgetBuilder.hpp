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
      const auto state = ptr->currentState();
      // Switch Textlabel color based on if some or all devices are on.
      using namespace mqtt;
      const auto textColor = (state == MQTTSwitchGroupState::Some ||
        state == MQTTSwitchGroupState::All) ? Color::ActiveBgColor() : Color::InactiveTextColor();
      const auto imagePath = ptr->isActive() ? icons.first : icons.second;
      button->setImage(imagePath);
      button->setTextColor(textColor);
      button->setMoreIndicator(ptr->mDevices.size() > 1);
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
        using namespace mqtt;
        const auto state = ptr->currentState();
        // Switch Textlabel color based on if some or all devices are on.
        const auto textColor = (state == MQTTSwitchGroupState::Some ||
          state == MQTTSwitchGroupState::All) ? Color::ActiveBgColor() : Color::InactiveTextColor();
        // Image will only change color when all devices are on.
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
          case MQTTSensorType::MQTTTwoValues:
          {
            const auto firstValue = sensor.second.getFirstValue();
            const auto secondValue = sensor.second.getSecondValue();
            const auto secondValueIcon = util::GetSmallIcon(util::secondValueIcon);
            const auto firstValueIcon = util::GetSmallIcon(util::firstValueIcon);
            button->setImageWithValue({firstValueIcon, firstValue});
            button->setImageWithValue({secondValueIcon, secondValue});
            break;
          }

          case MQTTSensorType::MQTTValue:
          {
            const auto firstValue = sensor.second.getFirstValue();
            const auto firstValueIcon = util::GetSmallIcon(util::firstValueIcon);
            button->setImageWithValue({firstValueIcon, firstValue});
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