#pragma once

#include <config/Fonts.hpp>
#include <config/PlatformInject.hpp>
#include <touch/TouchTypes.hpp>
#include <ui/Color.hpp>
#include <ui/UIPosition.hpp>

#include <array>
#include <memory>
#include <optional>
#include <vector>

namespace gfx
{

class UIWidget;
using WidgetPtr = std::shared_ptr<UIWidget>;
class UIWidget : public std::enable_shared_from_this<UIWidget>
{
  public:
    UIWidget() = default;
    UIWidget(UIWidget&&) = default;
  
    UIWidget(ScreenDriver* screenptr, Frame frame, uint16_t tag = 0);
    UIWidget(ScreenDriver* screenptr, std::weak_ptr<UIWidget> parent, Frame frame, uint16_t tag = 0);
    virtual void draw() { };
    virtual void addSubview(WidgetPtr widget);
    virtual void setFrame(Frame frame) { mFrame = frame; };
    virtual void setBackgroundColor(Color color) { mBackgroundColor = color; mNeedsRedraw = true; };
    virtual std::vector<WidgetPtr> &getSubViews() { return mpSubViews; }
    virtual void didTap(const TapEvent& tapEvt);
    virtual void setNeedsRedraw();
    virtual void setRedrawSubviews() {};
    virtual void setParent(std::weak_ptr<UIWidget> parent);

    virtual int getNumSubviews();

    // Button Functions
    virtual void didTap(const ButtonEvent& btnEvt) {};
    virtual void setSelected(const bool selected);
    virtual bool isSelected() const { return mIsSelected; }
    virtual void select() {};


  protected:
    template <typename Derived>
    std::shared_ptr<Derived> shared_from_base()
    {
        return std::static_pointer_cast<Derived>(shared_from_this());
    };
    Color getRandomColor();
    ScreenDriver* mpScreen;
    std::weak_ptr<UIWidget> mpParent;
    Frame mFrame;
    TouchEvent mLastEvent;
    unsigned long mLastTouchEventTime = 0;
    const uint16_t mTag;
    Color mBackgroundColor;
    std::vector<WidgetPtr> mpSubViews;
    bool mNeedsRedraw = true;
    bool mIsFirstDraw = true;
    bool mIsSelected = false;
};
} // namespace gfx
