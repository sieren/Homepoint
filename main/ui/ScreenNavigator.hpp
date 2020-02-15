#pragma once

#include <ui/UIButton.h>
#include <ui/UIWidget.hpp>

#include <config/Icon.hpp>
#include <ui/UIMosaicMenuWidget.hpp>
#include <ui/UIPageButton.h>
#include <util/stdextend.hpp>

#include <algorithm>
#include <functional>

namespace gfx
{
  template<typename NavigationDriver>
  class ScreenNavigator : public UIWidget
  {
    public:
      ScreenNavigator(ScreenDriver* screenptr, Frame frame, uint16_t tag) :
        UIWidget(screenptr, frame, tag)
        { };
      ScreenNavigator(ScreenDriver* screenptr, std::weak_ptr<UIWidget> parent, Frame frame, uint16_t tag) :
        UIWidget(screenptr, parent, frame, tag)
        { };

      void didTap(const TapEvent& tapEvt) override
      {
        if (mpSubViews.size() == 0)
        {
          return;
        }
        mpSubViews.at(mCurrentScreenPage)->didTap(tapEvt);
      }

      void draw() override
      {
        mpSubViews.at(mCurrentScreenPage)->draw();
      }

      void pageUp()
      {
        mCurrentScreenPage = mCurrentScreenPage + 1 > (mpSubViews.size() - 1) ? 0 : mCurrentScreenPage + 1;
        mpSubViews.at(mCurrentScreenPage)->setRedrawSubviews();
      }

      void pageDown()
      {
        mCurrentScreenPage = mCurrentScreenPage - 1 < 0 ? mpSubViews.size() - 1 : mCurrentScreenPage - 1;
        mpSubViews.at(mCurrentScreenPage)->setRedrawSubviews();
      }

      void didTap(const ButtonEvent& btnEvt) override
      {
        int buttonInc = 0;
        switch (btnEvt.button)
        {
          case Button::A:
            buttonInc = -1;
            break;
          case Button::B:
            buttonInc = 1;
            break;
          case Button::C:
            handleSelectionEvent(btnEvt);
            return;
          default:
            buttonInc = 0;
        }
        
        auto& currentViews = mpSubViews.at(mCurrentScreenPage)->getSubViews();
        mCurrentButtonSelected += buttonInc;

        std::for_each(currentViews.begin(), currentViews.end(), [](const auto& btn)
        {
          btn->setSelected(false);
        });
        if (mCurrentButtonSelected > static_cast<int>(currentViews.size() - 1))
        {
          pageUp();
          mCurrentButtonSelected = 0;
        }
        else if (mCurrentButtonSelected < 0)
        {
          pageDown();
          mCurrentButtonSelected = mpSubViews.at(mCurrentScreenPage)->getSubViews().size() - 1;
        }
        if (currentViews.size() == 0)
        {
          return;
        }
        mpSubViews.at(mCurrentScreenPage)->getSubViews().at(mCurrentButtonSelected)->setSelected(true);
      }

      void addSubviews(std::vector<WidgetPtr> widget, const bool needsDismissal = false)
      {
        const int buttonSpaces = needsDismissal ? 5 : 6;
        const auto needsPageButton = widget.size() > buttonSpaces;

        auto makeMosaicButton = [&]()
        {
          auto menuWidget = std::make_shared<UIMosaicMenuWidget>(mpScreen, shared_from_this(), mFrame, 3, 2, 998);
          menuWidget->setBackgroundColor(Color::BlackColor());
          return menuWidget;
        };
        
        auto makePageButton = [&]()
        {
          auto pageButton = std::make_shared<UIPageButton>(mpScreen, mFrame);
          // Capturing the class by reference causes
          // a segfault in the ESP32 toolchain
          pageButton->addPageUpAction([this](auto i){
            this->pageUp();
          });
          pageButton->addPageDownAction([this](auto i){
            this->pageDown();
          });
          return pageButton;
        };
        
        if (needsPageButton)
        {
          const int widgetLimit = isTouchDriver ? 5 : 6;
          auto it = widget.begin();
          while (std::distance(it, widget.end()) > widgetLimit)
          {
            auto menuWidget = makeMosaicButton();
            auto nextIt = it;
            if (nextIt == widget.begin() && !isTouchDriver)
            {
              (*nextIt)->setSelected(true);
            }
            ::util::safe_advance(nextIt, widget.end(), widgetLimit);
            std::for_each(it, nextIt, [&](auto ele)
            {
              menuWidget->addSubview(ele);
            });
            it = nextIt;
            if (isTouchDriver)
            {
              menuWidget->addSubview(makePageButton());
            }
            mpSubViews.push_back(menuWidget);
          }
          // Add remaining views
          if (std::distance(it, widget.end()) > 0)
          {
            auto menuWidget = makeMosaicButton();
            std::for_each(it, widget.end(), [&](auto ele)
            {
              menuWidget->addSubview(ele);
            });
            if (isTouchDriver)
            {
              menuWidget->addSubview(makePageButton());
            }
            mpSubViews.push_back(menuWidget);
          }
        }
        else
        {
          auto menuWidget = makeMosaicButton();
          std::for_each(widget.begin(), widget.end(), [&](auto ele)
          {
            menuWidget->addSubview(ele);
          });
          mpSubViews.push_back(menuWidget);
        }
      }

      void presentDismissalSubviews(std::vector<WidgetPtr> widget, ButtonCallback dismissCb)
      {
        if (!isTouchDriver)
        {
          (*widget.begin())->setSelected(true);
        }
        addSubviews(widget, true);
        auto& lastPage = mpSubViews.back();
        auto backButton = std::make_shared<UIButton>(mpScreen, mFrame);
        backButton->setLabel("Back");
        backButton->setTextColor(Color::WhiteColor());
        backButton->setImage(util::GetIconFilePath("exit"));
        auto thisTag = mTag;
        backButton->addTargetAction([dismissCb, &thisTag](const uint16_t tag)
        {
          dismissCb(thisTag);
        });

        if (lastPage->getNumSubviews() > 5)
        {
          auto menuWidget = std::make_shared<UIMosaicMenuWidget>(mpScreen, mFrame, 3, 2, 998);
          menuWidget->setBackgroundColor(Color::BlackColor());
          menuWidget->addSubview(std::move(backButton));
          UIWidget::addSubview(menuWidget);
        }
        else
        {
          lastPage->addSubview(backButton);
        }
        mIsPresenting = true;
      }

      void addSubview(WidgetPtr widget)
      {
        if ((mpSubViews.size() + 1) % 5 == 0)
        {
          // add Page button
        }
        UIWidget::addSubview(widget);
      }

    private:
      void handleSelectionEvent(const ButtonEvent& btnEvt)
      {
        auto& currentViews = mpSubViews.at(mCurrentScreenPage)->getSubViews();
        auto selectedButton = std::find_if(currentViews.begin(), currentViews.end(), [](auto& btn)
        {
          return btn->isSelected();
        });

        if (selectedButton != currentViews.end())
        {
          (*selectedButton)->didTap(btnEvt);
        }       
      }

      int mCurrentScreenPage = 0;
      bool mIsPresenting{false};
      // Check if we're using a TouchDriver at compile-time
      // to determine whether we need on-screen page buttons
      static constexpr bool isTouchDriver = std::is_same<NavigationDriver, gfx::TouchDriver>::value;
      int mCurrentButtonSelected = 0;


  };
} // namespace gfx
