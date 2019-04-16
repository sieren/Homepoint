#pragma once

#include "UIWidget.hpp"

namespace gfx
{
  class UIMosaicMenuWidget : public UIWidget
  {
    public:
      UIMosaicMenuWidget(ScreenDriver* screenptr, Frame frame, int cols = 3, int rows = 2, uint16_t tag = 0);
      UIMosaicMenuWidget(ScreenDriver* screenptr, std::weak_ptr<UIWidget> parent, Frame frame, int cols = 3, int rows = 2, uint16_t tag = 0);
      void addSubview(WidgetPtr widget) override;
      void draw() override;
      void setRedrawSubviews() override;

    private:
      void drawGrid();
      int mCols;
      int mRows;
  };
}