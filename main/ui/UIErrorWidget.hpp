#pragma once

#include <ui/UIWidget.hpp>
#include <string>

namespace gfx
{
  static const int kLabelOffset = 5; // for centering

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
      if (mWarningMessage.length() > 40)
      {
        std::string firstLine = mWarningMessage.substr(0, 40);
        const auto textWidth = mpScreen->getTextWidth(firstLine.c_str());
        const auto centerPoint = mFrame.getCenterPoint();
        textFrame.position.x = centerPoint.x - textWidth / 2;
        textFrame.position.y = mFrame.size.height/2 - kLabelOffset;
        mpScreen->drawText(textFrame, 1, firstLine.c_str());
        auto secondLine = mWarningMessage.substr(40, mWarningMessage.length() - 1);
        auto secondTextFrame = textFrame;
        secondTextFrame.position.y = textFrame.position.y + 10;
        mpScreen->drawText(secondTextFrame, 1, secondLine.c_str());
      }
      else
      {
        const auto textWidth = mpScreen->getTextWidth(mWarningMessage.c_str());
        const auto centerPoint = mFrame.getCenterPoint();
        textFrame.position.x = centerPoint.x - textWidth / 2;
        textFrame.position.y = mFrame.size.height/2 - kLabelOffset;
        mpScreen->drawText(textFrame, 1, mWarningMessage.c_str());
      }
      mpScreen->pushSprite(mFrame.position);
      mpScreen->deleteSprite();
    }

    private:
      std::string mWarningMessage;
  };
}