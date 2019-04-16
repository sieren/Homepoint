#include <ui/UIPageButton.h>

#include <config/Icon.hpp>
#include <sound/BeepGenerator.hpp>

namespace gfx
{
  UIPageButton::UIPageButton(ScreenDriver* screenptr, Frame frame, uint16_t tag) :
    UIWidget(screenptr, frame, tag)
  { }

  UIPageButton::UIPageButton(ScreenDriver* screenptr, std::weak_ptr<UIWidget> parent, Frame frame, uint16_t tag) :
    UIWidget(screenptr, parent, frame, tag)
  { }

  void UIPageButton::didTap(const TapEvent& tapEvt)
  {
    if (mFrame.isInBounds(tapEvt.position)
        && tapEvt.state == PressEvent::Tap)
    {
      mNeedsRedraw = true;

      const auto center = mFrame.getCenterPoint();
      const auto isLeft = tapEvt.position.x < mFrame.position.x + center.x;

      if (isLeft && mDownCallback)
      {
        sound::BeepGenerator::ShortBeep();
        mDownCallback(mTag);
      } 
      else if (mUpCallback)
      {
        sound::BeepGenerator::ShortBeep();
        mUpCallback(mTag);
      }
      return;
    }
  }

  void UIPageButton::addPageUpAction(ButtonCallback cb)
  {
    mUpCallback = cb;
  }

  void UIPageButton::addPageDownAction(ButtonCallback cb)
  {
    mDownCallback = cb;
  }

  void UIPageButton::draw()
  {
    if (!mNeedsRedraw)
    {
      return;
    }
    mpScreen->createSprite(mFrame, mBackgroundColor);
    Point imagePosition;
    imagePosition.x = mFrame.getCenterPoint().x - 50 + 25/2;
    imagePosition.y = mFrame.getCenterPoint().y - 25 / 2;
    ImageWriter::drawJpeg(mpScreen, util::GetIconFilePath(mLeftImage), imagePosition);
    imagePosition.x = mFrame.getCenterPoint().x + 25/2;
    ImageWriter::drawJpeg(mpScreen, util::GetIconFilePath(mRightImage), imagePosition);
    mpScreen->pushSprite(mFrame.position);
    mpScreen->deleteSprite();
    mNeedsRedraw = false;
  }
} // namespace gfx
