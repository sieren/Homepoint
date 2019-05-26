#pragma once

#include "UIWidget.hpp"

#include <vector>
#include <utility>

// Combo Widget, supports up to two values on the
// home screen

namespace gfx
{
  class UISensorComboWidget : public UIWidget
  {
    using ImagePath = std::string;
    using ValueType = std::string;
    public:
      UISensorComboWidget(ScreenDriver* screenptr, Frame frame, uint16_t tag = 0);

      void draw() override;
      void setTextColor(Color textColor);
      void setLabel(const std::string label);
      void setImageWithValue(const std::pair<ImagePath, ValueType>& valuePair);
      void eraseValues();

    private:
      Color mTextColor;
      std::string mLabel;
      std::vector<std::pair<ImagePath, ValueType>> mValues;
  };
} // namespace gfx
