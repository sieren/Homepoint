#include "UIButton.h"
#include <config/Config.h>
#include <sound/BeepGenerator.hpp>
#include <ui/Color.hpp>

#include <string>
#include <sstream>
#include <iostream>
#include <sstream>

namespace gfx
{
  UIButton::UIButton(ScreenDriver* screenPtr, Frame frame, const uint16_t tag) :
    UIWidget(screenPtr, frame, tag)
    {
      mBackgroundColor = Color::BlackColor();
    }
  
   UIButton::UIButton(ScreenDriver* screenPtr, std::weak_ptr<UIWidget> parent, Frame frame, const uint16_t tag) :
    UIWidget(screenPtr, parent, frame, tag)
  {
    mBackgroundColor = Color::BlackColor();
  }

  void UIButton::draw()
  {
      if (!mNeedsRedraw)
      {
        return;
      }
      std::for_each(mpSubViews.begin(), mpSubViews.end(), [](auto& subView) {
        subView->draw();
      });
      mpScreen->createSprite(mFrame, mBackgroundColor);
      Point imagePosition;
      imagePosition.x = mFrame.getCenterPoint().x - 50 / 2;
      imagePosition.y = 20;
      ImageWriter::drawJpeg(mpScreen, mImagePath, imagePosition);
      mpScreen->loadFont(SmallFont);
      mpScreen->setTextColor(mTextColor, mBackgroundColor);
      Frame textFrame;
      const auto textWidth = mpScreen->getTextWidth(mLabel.c_str());
      const auto centerPoint = mFrame.getCenterPoint();
      textFrame.position.x = centerPoint.x - textWidth / 2;
      textFrame.position.y = mFrame.size.height - 30;

      const auto activeColor = mIsSelected ? Color::SelectedColor() : mTextColor; 
      mpScreen->setTextColor(activeColor, mBackgroundColor);
      mpScreen->drawText(textFrame, 1, mLabel.c_str());

      if (mHasMoreIndicator)
      {
        Frame moreFrame;
        const std::string moreLabel = "...";
        const auto textWidth = mpScreen->getTextWidth(moreLabel.c_str());
        moreFrame.position.x = mFrame.size.width - textWidth - 13;
        moreFrame.position.y = 10;
        mpScreen->loadFont(BoldFont);
        mpScreen->drawText(moreFrame, 1, moreLabel.c_str());
      }

      mNeedsRedraw = false;
      mpScreen->pushSprite(mFrame.position);
      mpScreen->deleteSprite();
  }

  void UIButton::setImage(const std::string filePath)
  {
    mImagePath = filePath;
    UIWidget::setNeedsRedraw();
  }

  void UIButton::setMoreIndicator(const bool hasMore)
  {
    mHasMoreIndicator = hasMore;
  }

  void UIButton::setTextColor(Color textColor)
  {
    mTextColor = textColor;
    UIWidget::setNeedsRedraw();
  }

  void UIButton::setLabel(const std::string label)
  {
    mLabel = label;
    UIWidget::setNeedsRedraw();
  }
  
  void UIButton::addTargetAction(ButtonCallback cb)
  {
    mButtonCallback = cb;
  }

  void UIButton::addTargetLongPressAction(ButtonCallback cb)
  {
    mButtonLongPressCallback = cb;
  }

  void UIButton::didTap(const TapEvent& tapEvt)
  {
    if (mFrame.isInBounds(tapEvt.position))
    {
      auto mUpperFrame = mFrame;
      mUpperFrame.size.height = mUpperFrame.size.height / 2;
      const auto didTapMoreArea = mUpperFrame.isInBounds(tapEvt.position);
      if (didTapMoreArea && mHasMoreIndicator)
      {
        callLongPressAction();
      }
      else
      {
        callTapAction();
      }
    }
  }

  void UIButton::didTap(const ButtonEvent& btnEvt)
  {
    switch (btnEvt.state)
    {
      case ButtonPress::Tap:
        callTapAction();
        break;
      case ButtonPress::LongPress:
        callLongPressAction();
        break;
    }
  }

  void UIButton::callLongPressAction()
  {
    if (mButtonLongPressCallback)
    {
      sound::BeepGenerator::ShortBeep();
      mButtonLongPressCallback(mTag);
    }
  }

  void UIButton::callTapAction()
  {
    if (mButtonCallback)
    {
      sound::BeepGenerator::ShortBeep();
      mButtonCallback(mTag);
    }
  }
} // namespace gfx
