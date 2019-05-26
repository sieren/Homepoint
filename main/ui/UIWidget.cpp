#pragma once
#include "Color.hpp"
#include "UIWidget.hpp"

namespace gfx
{
    UIWidget::UIWidget(ScreenDriver* screenptr, Frame frame, const uint16_t tag):
      mpScreen(screenptr),
      mFrame(frame),
      mTag(tag)
    {
      mBackgroundColor = Color::BlackColor();
    };

    UIWidget::UIWidget(ScreenDriver* screenptr, std::weak_ptr<UIWidget> parent,
      Frame frame, uint16_t tag) :
        mpScreen(screenptr),
        mpParent(parent),
        mFrame(frame),
        mTag(tag)
    {

    }

    void UIWidget::addSubview(WidgetPtr widget)
    {
      widget->mFrame = mFrame.frameInBounds(widget->mFrame);
      mpSubViews.push_back(widget);
    }

    void UIWidget::didTap(const TapEvent& tapEvt)
    {
      std::for_each(mpSubViews.begin(), mpSubViews.end(), [&tapEvt](auto& subView) {
        subView->didTap(tapEvt);
      });
    }

    void UIWidget::setNeedsRedraw()
    {
      mNeedsRedraw = true;
      auto parent = mpParent.lock();
      if (!parent)
      {
        return;
      }
      parent->setNeedsRedraw();
    }

    int UIWidget::getNumSubviews()
    {
      return mpSubViews.size();
    }

    void UIWidget::setParent(std::weak_ptr<UIWidget> parent)
    {
      mpParent = parent;
    }

    void UIWidget::setSelected(const bool selected)
    {
      if (mIsSelected != selected)
      {
        mIsSelected = selected;
        setNeedsRedraw();
      }
    }


} // namespace gfx
