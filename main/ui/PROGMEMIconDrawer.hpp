#pragma once

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
  // TODO: Delete this.
  // Previously used to save icons in program code for faster loading
  // compared to SPIFFS
  using ImageItem = std::tuple<std::string, const uint8_t*, size_t>;
  static std::vector<ImageItem> imageMapping =
  {
  };

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
