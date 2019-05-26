#include "UISensorComboWidget.h"

#include <config/Icon.hpp>
#include <util/stdextend.hpp>

namespace gfx
{
  UISensorComboWidget::UISensorComboWidget(ScreenDriver* screenptr, Frame frame, uint16_t tag) :
    UIWidget(screenptr, frame, tag)
    {
      
    }

  void UISensorComboWidget::draw()
  {
    if (!mNeedsRedraw)
    {
      return;
    }
    std::for_each(mpSubViews.begin(), mpSubViews.end(), [](auto& subView) {
      subView->draw();
    });
    if (mValues.size() == 0)
    {
      return;
    }
    mpScreen->createSprite(mFrame, mBackgroundColor);
    mpScreen->loadFont(SmallFont);
    const auto textColor = mIsSelected ? Color::SelectedColor() : mTextColor;
    mpScreen->setTextColor(textColor, mBackgroundColor);
    Frame textFrame;
    const auto textWidth = mpScreen->getTextWidth(mLabel.c_str());
    const auto centerPoint = mFrame.getCenterPoint();
    textFrame.position.x = centerPoint.x - textWidth / 2;
    textFrame.position.y = mFrame.size.height - 30;
    mpScreen->drawText(textFrame, 1, mLabel.c_str());

    mpScreen->unloadFont();
    mpScreen->loadFont(BoldFont);
    mpScreen->setTextColor(mTextColor, mBackgroundColor);
  
    const auto isMultiSensor = mValues.size() > 1;
    // Aligning by first label
    auto firstImage = mValues.at(0).first;
    auto firstValue = mValues.at(0).second;
    const auto firstWidth = mpScreen->getTextWidth(estd::to_string(firstValue).c_str());
    const auto labelPosition = centerPoint.x - (firstWidth + util::SmallIconSize + util::SmallIconInsetToCenter) / 2;
    Frame firstFrame;
    firstFrame.position.x = labelPosition + util::SmallIconSize;
    firstFrame.position.y += isMultiSensor ? 20 : centerPoint.y - 20;

    auto firstImagePosition = firstFrame.position;
    firstImagePosition.x = firstFrame.position.x - util::SmallIconSize;
    firstImagePosition.y -= 4;
    ImageWriter::drawJpeg(mpScreen, firstImage, firstImagePosition);
    mpScreen->drawText(firstFrame, 1, firstValue);

    if (isMultiSensor)
    {
      auto secondImage = mValues.at(1).first;
      auto secondValue = mValues.at(1).second;
      auto secondFrame = firstFrame;
      secondFrame.position.y += 30;
      auto secondImagePosition = secondFrame.position;
      secondImagePosition.x = secondFrame.position.x - util::SmallIconSize;
      secondImagePosition.y -= 4;
      ImageWriter::drawJpeg(mpScreen, secondImage, secondImagePosition);
      mpScreen->drawText(secondFrame, 1, secondValue);
    }

    mNeedsRedraw = false;
    mpScreen->pushSprite(mFrame.position);
    mpScreen->unloadFont();
    mpScreen->deleteSprite();
  }

  void UISensorComboWidget::setImageWithValue(const std::pair<ImagePath, ValueType>& valuePair)
  {
    mValues.push_back(std::move(valuePair));
    mNeedsRedraw = true;
  }

  void UISensorComboWidget::eraseValues()
  {
    mValues.clear();
  }

  void UISensorComboWidget::setLabel(const std::string label)
  {
    mLabel = label;
    mNeedsRedraw = true;
  }

  void UISensorComboWidget::setTextColor(Color color)
  {
    mTextColor = color;
  }
} // namespace gfx
