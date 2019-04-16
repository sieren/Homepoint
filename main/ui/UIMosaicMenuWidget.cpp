#include "UIMosaicMenuWidget.hpp"

namespace gfx
{

  UIMosaicMenuWidget::UIMosaicMenuWidget(ScreenDriver* screenptr, Frame frame, int cols, int rows, uint16_t tag) :
    UIWidget(screenptr, frame, tag),
    mCols(cols),
    mRows(rows)
    {
      
    }
  

  UIMosaicMenuWidget::UIMosaicMenuWidget(ScreenDriver* screenptr, std::weak_ptr<UIWidget> parent, Frame frame, int cols, int rows, uint16_t tag) :
    UIWidget(screenptr, parent, frame, tag),
    mCols(cols),
    mRows(rows)
    {
      
    }

  void UIMosaicMenuWidget::addSubview(WidgetPtr widget)
  {
    widget->setParent(shared_from_this());
    UIWidget::addSubview(widget);
    int w = std::ceil<int>(mFrame.size.width/mCols) + 1;
    int h = mFrame.size.height/mRows;

    int mult = mpSubViews.size() - 1;
    int hPos = mult/mCols;
    int xPos = w*((mpSubViews.size()-1)%3) + mFrame.position.x;
    int yPos = h*hPos + mFrame.position.y;
    const auto frame = Frame{{xPos, yPos, 0}, {w, h}};
    mpSubViews.back()->setFrame(frame);
  }

  void UIMosaicMenuWidget::draw()
  {
    if (mIsFirstDraw)
    {
      // Draw background once
      mpScreen->createSprite(mFrame, mBackgroundColor);
      mpScreen->pushSprite(mFrame.position);
      mpScreen->deleteSprite();
      mIsFirstDraw = false;
    }
    
    std::for_each(mpSubViews.begin(), mpSubViews.end(), [](auto& subView)
    {
      subView->draw();
    });
    if (mNeedsRedraw)
    {
      drawGrid();
    }
  }

  void UIMosaicMenuWidget::drawGrid()
  {
    for (int h = 1; h < mRows; h++)
    {
      auto horizontalLine = mFrame;
      horizontalLine.size.height = 1;
      horizontalLine.position.y = mFrame.position.y + h * std::floor(mFrame.size.height / mRows);
      mpScreen->createSprite(horizontalLine, Color::LightGrayColor());
      mpScreen->pushSprite(horizontalLine.position);
      mpScreen->deleteSprite();
    }
    for (int v = 1; v < mCols; v++)
    {
      auto verticalLine = mFrame;
      int w = std::ceil<int>(mFrame.size.width/mCols) + 1;
      verticalLine.size.width = 1;
      verticalLine.position.x = mFrame.position.x + v * w;
      mpScreen->createSprite(verticalLine, Color::LightGrayColor());
      mpScreen->pushSprite(verticalLine.position);
      mpScreen->deleteSprite();
    }
  }

  void UIMosaicMenuWidget::setRedrawSubviews()
  {
    std::for_each(mpSubViews.begin(), mpSubViews.end(), [](auto& subView)
    {
      subView->setNeedsRedraw();
    });
    mIsFirstDraw = true;
  }
} // namespace gfx