#include "UIMosaicButton.h"

#include "AppNewScreen.h"
#include <ui/Icons.hpp>
#include <ui/Styles.h>
#include <config/Config.h>
#include <config/Icon.hpp>
#include <ntp/NTPSync.h>
#include <functional>

namespace gfx
{

  UIMosaicButton::UIMosaicButton(lv_obj_t *parent, const std::string label, const uint16_t tagId) :
    mpParent(parent),
    mTagId(tagId),
    mLabel(label)
  {
  }

  void UIMosaicButton::setupButton()
  {
    mpContHeader = lv_cont_create(mpParent, NULL);
    lv_cont_set_fit2(mpContHeader, LV_FIT_NONE, LV_FIT_NONE);
    int width = lv_obj_get_width(mpParent);
    int height = lv_obj_get_width(mpParent);
    lv_obj_set_height(mpContHeader, width/3 - 10);
    lv_obj_set_width(mpContHeader, width/3);
    lv_cont_set_layout(mpContHeader, LV_LAYOUT_COLUMN_MID);
    lv_obj_align(mpContHeader, mpParent, LV_ALIGN_CENTER, 0, 0);

    static lv_style_t innerStyle;
    lv_style_copy(&innerStyle, &Styles::getInstance().mainStyle);
    lv_style_set_pad_bottom(&innerStyle, LV_STATE_DEFAULT, 1);
    lv_obj_add_style(mpContHeader, LV_OBJ_PART_MAIN, &Styles::getInstance().switchBtnContStyle);

    mpImage = lv_img_create(mpContHeader, NULL);
    lv_page_glue_obj(mpContHeader, true);
    lv_obj_set_user_data(mpContHeader, this);
    lv_obj_add_style(mpImage, LV_OBJ_PART_MAIN, &Styles::getInstance().switchBtnContStyle);

    mpLabelText = lv_label_create(mpContHeader, NULL);
    static lv_style_t style_status_label;
    lv_style_copy(&style_status_label, &innerStyle);
    lv_style_set_text_color(&style_status_label, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_add_style(mpLabelText, LV_BTN_PART_MAIN, &style_status_label);
    lv_obj_set_width(mpLabelText, lv_obj_get_width(mpContHeader));
    lv_label_set_text(mpLabelText, mLabel.c_str());
    lv_label_set_align(mpLabelText, LV_LABEL_ALIGN_CENTER);
    lv_obj_set_style_local_pad_top(mpLabelText, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 5);
    lv_obj_align(mpLabelText, mpContHeader, LV_ALIGN_IN_BOTTOM_MID, 0, 0);

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

  void UIMosaicButton::setImage(const std::string imageName)
  {
    ui::resolveAndSetImage(mpImage, imageName);
  }

  void UIMosaicButton::setTextLabel(const std::string label)
  {
    mLabel = label;
    lv_label_set_text(mpLabelText, mLabel.c_str());
  }

  void UIMosaicButton::setState(const bool isActive)
  {
    if (isActive) {
      lv_obj_set_style_local_image_recolor_opa(mpImage, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 200);
      lv_obj_set_style_local_image_recolor(mpImage, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
      return;
    }
    lv_obj_set_style_local_image_recolor_opa(mpImage, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_image_recolor(mpImage, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  }

  void UIMosaicButton::setState(const mqtt::MQTTSwitchGroupState state)
  {
    using namespace mqtt;
    switch (state) {
      case MQTTSwitchGroupState::None:
      lv_obj_set_style_local_image_recolor_opa(mpImage, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 0);
      lv_obj_set_style_local_image_recolor(mpImage, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
      lv_obj_set_style_local_text_color(mpLabelText, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
      break;

      case MQTTSwitchGroupState::Some:
      lv_obj_set_style_local_image_recolor_opa(mpImage, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 0);
      lv_obj_set_style_local_image_recolor(mpImage, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
      lv_obj_set_style_local_text_color(mpLabelText, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
      break;

      case MQTTSwitchGroupState::All:
      lv_obj_set_style_local_image_recolor_opa(mpImage, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, 200);
      lv_obj_set_style_local_image_recolor(mpImage, LV_IMG_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
      lv_obj_set_style_local_text_color(mpLabelText, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
    }
    
  }

  void UIMosaicButton::addTargetAction(ButtonCallback cb)
  {
    mButtonCallback = cb;
  }

  void UIMosaicButton::addTargetLongPressAction(ButtonCallback cb)
  {
    mButtonLongPressCallback = cb;
  }

  uint16_t UIMosaicButton::getId()
  {
    return mTagId;
  }

} // namespace gfx
