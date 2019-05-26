#pragma once

#include <ui/PROGMEMIcons.h>
#include <ui/SPIFFSIconDrawer.hpp>
#include <ui/UIPosition.hpp>

#include <map>
#include <string>
#include <vector>
#include <tuple>


namespace gfx
{
namespace util
{
  using ImageItem = std::tuple<std::string, const uint8_t*, size_t>;
      static std::vector<ImageItem> imageMapping =
      {ImageItem{"/bedroom_active.jpg", bedroom_active, sizeof(bedroom_active)},
      ImageItem{"/bedroom_inactive.jpg", bedroom_inactive, sizeof(bedroom_inactive)},
      ImageItem{"/livingroom_active.jpg", livingroom_active, sizeof(livingroom_active)},
      ImageItem{"/livingroom_inactive.jpg", livingroom_inactive, sizeof(livingroom_inactive)},
      ImageItem{"/arrow_left.jpg", arrow_left, sizeof(arrow_left)},
      ImageItem{"/arrow_right.jpg", arrow_right, sizeof(arrow_right)},
      ImageItem{"/door_active.jpg", door_active,sizeof(door_active)},
      ImageItem{"/door_inactive.jpg", door_inactive,sizeof(door_inactive)},
      ImageItem{"/power_active.jpg", power_active,sizeof(power_active)},
      ImageItem{"/power_inactive.jpg", power_inactive,sizeof(power_inactive)},
      ImageItem{"/fan_active.jpg", fan_active,sizeof(fan_active)},
      ImageItem{"/fan_inactive.jpg", fan_inactive,sizeof(fan_inactive)},
      ImageItem{"/humidity_small.jpg", humidity_small,sizeof(humidity_small)},
      ImageItem{"/temperature_small.jpg", temperature_small,sizeof(temperature_small)},
      ImageItem{"/voc_small.jpg", voc_small,sizeof(voc_small)},
      ImageItem{"/exit.jpg", exit_icon,sizeof(exit_icon)}};

  template<typename ScreenDriver>
  struct PROGMEMIconDrawer
  {
    static void drawJpeg(ScreenDriver* screenPtr, const std::string fileName, Point position)
    {
      const auto icon = std::find_if(imageMapping.begin(), imageMapping.end(), [&fileName](const auto& ele)
      {
        return std::get<0>(ele) == fileName;
      });
      if (icon == imageMapping.end())
      {
        printf("Couldn't resolve image ");
        printf(fileName.c_str());
        printf("\r\n");
        SPIFFSIconDrawer<ScreenDriver>::drawJpeg(screenPtr, fileName, position);
        return;
      }
      screenPtr->drawJpg(std::get<1>(*icon), std::get<2>(*icon), position);
    }

  };
} // namespace util
} // namespace gfx
