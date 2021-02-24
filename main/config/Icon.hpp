#pragma once

#include <string>
#include <utility>

namespace gfx
{
namespace util
{

  static const std::string firstValueIcon = "temperature";
  static const std::string secondValueIcon = "humidity";
  static const std::string vocIcon = "voc";

  static const int SmallIconSize = 25; // 25x25px
  static const int SmallIconInsetToCenter = 8; // Left + right inset to icon in square

  inline static const std::string GetIconFilePath(const std::string name)
  {
    const auto fullPath = "D:spiffs/" + name + ".bin";
    return fullPath;
  }

} // namespace util
} // namespace gfx
