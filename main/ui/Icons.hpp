#pragma once

#include <config/Icon.hpp>
#include <ui/icons/IconIncludes.hpp>
#include <lvgl.h>

#include <algorithm>
#include <map>
#include <string>
#include <vector>

namespace gfx::ui 
{
  using ImageItem = std::pair<std::string, const lv_img_dsc_t*>;
  static std::vector<ImageItem> imageMapping =
  {
    ImageItem{"bedroom", &bedroom_img},
    ImageItem{"door", &door_img},
    ImageItem{"fan", &fan_img},
    ImageItem{"fountain", &fountain_img},
    ImageItem{"garden", &garden_img},
    ImageItem{"kitchen", &kitchen_img},
    ImageItem{"livingroom", &livingroom_img},
    ImageItem{"night", &night_img},
    ImageItem{"power", &power_img}
  };

  static void resolveAndSetImage(lv_obj_t* obj, const std::string& imageName)
  {
    const auto iconIt = std::find_if(imageMapping.begin(), imageMapping.end(), [&imageName](const auto& ele)
    {
      return ele.first == imageName;
    });
    if (iconIt != imageMapping.end())
    {
      lv_img_set_src(obj, iconIt->second);
    }
    else // fallback to path file
    {
      const auto filePath = util::GetIconFilePath(imageName);
      lv_img_set_src(obj, filePath.c_str());
    }
  }

}