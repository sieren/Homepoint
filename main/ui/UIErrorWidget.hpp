#pragma once

#include <ui/UIWidget.hpp>
#include <string>

namespace gfx
{
  struct UIErrorWidget : public UIWidget
  {
    UIErrorWidget(ScreenDriver* screenPtr, Frame frame, const uint16_t tag = 0) :
      UIWidget(screenPtr, frame, tag)
    {
      mBackgroundColor = {255, 0, 0};
    }

    void setWarningMessage(const std::string message)
    {
      mWarningMessage = message;
    }

    void draw() override
    {
      mpScreen->createSprite(mFrame, mBackgroundColor);
      mpScreen->setTextColor(Color::WhiteColor(), mBackgroundColor);
      Frame textFrame;
      const auto textWidth = mpScreen->getTextWidth(mWarningMessage.c_str());
      const auto centerPoint = mFrame.getCenterPoint();
      textFrame.position.x = centerPoint.x - textWidth / 2;
      textFrame.position.y = mFrame.size.height/2;

      mpScreen->drawText(textFrame, 1, mWarningMessage.c_str());
      mpScreen->pushSprite(mFrame.position);
      mpScreen->deleteSprite();
    }

    private:
      std::string mWarningMessage;
  };
}