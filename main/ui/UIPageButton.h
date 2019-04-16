#include <ui/UIWidget.hpp>
#include <ui/UIButton.h>

namespace gfx
{
  class UIPageButton : public UIWidget
  {
    public:
      UIPageButton(ScreenDriver* screenptr, Frame frame, uint16_t tag = 0);
      UIPageButton(ScreenDriver* screenptr, std::weak_ptr<UIWidget> parent, Frame frame, uint16_t tag = 0);
      void draw() override;
      void didTap(const TapEvent& tapEvt) override;

      void addPageUpAction(ButtonCallback cb);
      void addPageDownAction(ButtonCallback cb);
      
    private:
      TouchEvent mLastEvent;
      ButtonCallback mUpCallback;
      ButtonCallback mDownCallback;
      const std::string mLeftImage = "arrow_left";
      const std::string mRightImage = "arrow_right";

  };
} // namespace gfx