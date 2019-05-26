#pragma once

#include "UIWidget.hpp"

#include <atomic>
#include <functional>

using ButtonCallback = std::function<void(uint16_t)>;

namespace gfx
{
  class UIButton : public UIWidget
  {
    public:
    UIButton(ScreenDriver* screenPtr, Frame frame, const uint16_t tag = 0);
    UIButton(ScreenDriver* screenPtr, std::weak_ptr<UIWidget> parent, Frame frame, const uint16_t tag = 0);
    UIButton(ScreenDriver* screenPtr, const std::string tag);
    UIButton() = default;
    UIButton(UIButton&&) = default;
    UIButton(const UIButton&) = delete;
    UIButton& operator=(UIButton other) = delete;
    ~UIButton() = default;

    void addTargetAction(ButtonCallback cb);
    void addTargetLongPressAction(ButtonCallback cb);
    void setTextColor(Color textColor);
    void setImage(const std::string filePath);
    void setLabel(const std::string label);
    void draw() override;

    // Touch Driver
    void didTap(const TapEvent& tapEvt) override;

    // Button Driver
    void didTap(const ButtonEvent& btnEvt) override;

    private:
      void callLongPressAction();
      void callTapAction();
      Color mTextColor;
      std::string mImagePath;
      std::string mLabel;
      unsigned long mLastTouchEventTime;
      ButtonCallback mButtonCallback;
      ButtonCallback mButtonLongPressCallback;
  };
} // namespace gfx
