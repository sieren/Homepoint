#include "UISwitchDeviceButton.h"

#include "AppNewScreen.h"
#include <config/Config.h>
#include <config/Icon.hpp>
#include <ntp/NTPSync.h>
#include <functional>

namespace gfx
{

  UISwitchDeviceButton::UISwitchDeviceButton(lv_obj_t *parent, const std::string label, const uint16_t sceneId) :
    UIMosaicButton(parent, label, sceneId)
  {
 
  }

  void UISwitchDeviceButton::setupButton()
  {
    lv_obj_t* mpContHeader = lv_cont_create(UIMosaicButton::mpParent, NULL);
    lv_cont_set_fit2(mpContHeader, LV_FIT_NONE, LV_FIT_NONE);
    int width = lv_obj_get_width(UIMosaicButton::mpParent);
    lv_obj_set_height(mpContHeader, width/3 - 10);
    lv_obj_set_width(mpContHeader, width/3);
    lv_cont_set_layout(mpContHeader, LV_LAYOUT_COLUMN_MID);
    lv_obj_align(mpContHeader, UIMosaicButton::mpParent, LV_ALIGN_IN_TOP_LEFT, 0, 0);

    static lv_style_t lv_style1;
    lv_style_copy(&lv_style1, &mainStyle);
    lv_style_set_bg_color(&lv_style1, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_bg_color(&lv_style1, LV_STATE_PRESSED, LV_COLOR_GRAY);
    lv_style_set_bg_color(&lv_style1, LV_STATE_FOCUSED, LV_COLOR_RED);
    lv_style_set_bg_color(&lv_style1, LV_STATE_FOCUSED | LV_STATE_PRESSED, lv_color_hex(0xf88));
    lv_style_set_border_width(&lv_style1, LV_STATE_DEFAULT, 0);
    lv_style_set_text_font(&lv_style1, LV_STATE_DEFAULT, &lv_font_montserrat_8);
    lv_style_set_pad_inner(&lv_style1, LV_STATE_DEFAULT, 2);
    lv_style_set_pad_top(&lv_style1, LV_STATE_DEFAULT, 20);
    lv_style_set_pad_bottom(&lv_style1, LV_STATE_DEFAULT, 5);
    lv_page_glue_obj(mpContHeader, true);

    static lv_style_t innerStyle;
    lv_style_copy(&innerStyle, &mainStyle);
    lv_style_set_pad_bottom(&innerStyle, LV_STATE_DEFAULT, 1);
    lv_obj_add_style(mpContHeader, LV_OBJ_PART_MAIN, &lv_style1);

    mpImage = lv_img_create(mpContHeader, NULL);
    lv_obj_set_user_data(mpContHeader, this);

    mpLabelText = lv_label_create(mpContHeader, NULL);
    static lv_style_t style_status_label;
    lv_style_copy(&style_status_label, &innerStyle);
    lv_style_set_text_color(&style_status_label, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_add_style(mpLabelText, LV_BTN_PART_MAIN, &style_status_label);
    lv_label_set_text(mpLabelText, mLabel.c_str());

    lv_obj_set_event_cb(mpContHeader, [](lv_obj_t * obj, lv_event_t event)
    {
      auto self = reinterpret_cast<decltype(this)>(lv_obj_get_user_data(obj));
      if (event == LV_EVENT_SHORT_CLICKED)
      {
        if (self->mButtonCallback.has_value())
        {
          self->mButtonCallback.value()(self->getId());
        }
      }
      if (event == LV_EVENT_LONG_PRESSED)
      {
        if (self->mButtonLongPressCallback.has_value())
        {
          self->mButtonLongPressCallback.value()(self->getId());
        }
      }
    });
  }

  void UISwitchDeviceButton::setTextLabel(const std::string label)
  {
    mLabel = label;
    lv_label_set_text(mpLabelText, mLabel.c_str());
  }

  void UISwitchDeviceButton::addTargetAction(ButtonCallback cb)
  {
    mButtonCallback = cb;
  }

  void UISwitchDeviceButton::addTargetLongPressAction(ButtonCallback cb)
  {
    mButtonLongPressCallback = cb;
  }

} // namespace gfx
