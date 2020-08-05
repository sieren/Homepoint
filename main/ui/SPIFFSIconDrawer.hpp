#pragma once

#include <ui/UIPosition.hpp>

#include <string>


namespace gfx
{
namespace util
{
  template<typename ScreenDriver>
  struct SPIFFSIconDrawer
  {
    static void drawJpeg(ScreenDriver* screenPtr, const std::string fileName, Point position)
    {
      screenPtr->drawJpg(fileName, position);
    }
  };

} // namespace util
} // namespace gfx
