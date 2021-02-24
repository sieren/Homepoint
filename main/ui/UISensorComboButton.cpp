#include "UISensorComboButton.h"

#include "AppNewScreen.h"
#include <config/Config.h>
#include <config/Icon.hpp>
#include <ui/Icons.hpp>
#include <functional>

namespace gfx
{

  UISensorComboButton::UISensorComboButton(lv_obj_t *parent, const std::string label, const uint16_t id, const bool isMultiSensor) :
    UIMosaicButton(parent, label, id),
    isMultiSensor(isMultiSensor)
  {
 
  }

  void UISensorComboButton::setupButton()
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
    lv_style_set_text_font(&lv_style1, LV_STATE_DEFAULT, &lv_font_montserrat_12);
    lv_style_set_pad_inner(&lv_style1, LV_STATE_DEFAULT, 2);
    lv_style_set_pad_top(&lv_style1, LV_STATE_DEFAULT, 18);
    lv_style_set_pad_bottom(&lv_style1, LV_STATE_DEFAULT, 5);
    lv_page_glue_obj(mpContHeader, true);

    static lv_style_t innerStyle;
    lv_style_copy(&innerStyle, &mainStyle);
    lv_style_set_pad_top(&innerStyle, LV_STATE_DEFAULT, 1);
    lv_style_set_pad_bottom(&innerStyle, LV_STATE_DEFAULT, 1);
    lv_style_set_pad_left(&innerStyle, LV_STATE_DEFAULT, 4);
    lv_style_set_pad_right(&innerStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_border_width(&innerStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_bg_color(&innerStyle, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_obj_add_style(mpContHeader, LV_OBJ_PART_MAIN, &lv_style1);
  
    lv_obj_t* firstCont = lv_cont_create(mpContHeader, NULL);
    lv_cont_set_fit2(firstCont, LV_FIT_TIGHT, LV_FIT_TIGHT);
    lv_cont_set_layout(firstCont, LV_LAYOUT_ROW_MID);
    lv_obj_add_style(firstCont, LV_OBJ_PART_MAIN, &innerStyle);
    lv_obj_align(firstCont, mpContHeader, LV_ALIGN_IN_LEFT_MID, 0, 0);
    lv_obj_align_origo(firstCont, mpContHeader, LV_ALIGN_IN_LEFT_MID, 0, 0);

    static lv_style_t style_status_label;
    lv_style_copy(&style_status_label, &innerStyle);
    lv_style_set_text_color(&style_status_label, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&style_status_label, LV_STATE_DEFAULT, &lv_font_montserrat_12);

    static lv_style_t iconLabelStyle;
    lv_style_copy(&iconLabelStyle, &style_status_label);
    lv_style_set_pad_left(&innerStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_right(&innerStyle, LV_STATE_DEFAULT, 0);

    mpImage = lv_img_create(firstCont, NULL);
    lv_obj_add_style(mpImage, LV_OBJ_PART_MAIN, &innerStyle);
    lv_obj_align_origo(mpImage, firstCont, LV_ALIGN_IN_LEFT_MID, 0, 0);
    mpFirstLabel = lv_label_create(firstCont, NULL);
    lv_label_set_align(mpFirstLabel, LV_LABEL_ALIGN_LEFT);
    lv_obj_add_style(mpFirstLabel, LV_BTN_PART_MAIN, &iconLabelStyle);

    if (isMultiSensor)
    {
      lv_obj_t* secondCont = lv_cont_create(mpContHeader, NULL);
      lv_cont_set_fit2(secondCont, LV_FIT_TIGHT, LV_FIT_TIGHT);
      lv_cont_set_layout(secondCont, LV_LAYOUT_ROW_MID);
      lv_obj_add_style(secondCont, LV_OBJ_PART_MAIN, &innerStyle);
      lv_obj_align(secondCont, mpContHeader, LV_ALIGN_IN_LEFT_MID, 0, 0);
      lv_obj_align_origo(secondCont, mpContHeader, LV_ALIGN_IN_LEFT_MID, 0, 0);

      mpSecondImage = lv_img_create(secondCont, NULL);
      lv_obj_add_style(mpSecondImage, LV_OBJ_PART_MAIN, &innerStyle);
      lv_obj_align_origo(mpSecondImage, secondCont, LV_ALIGN_IN_LEFT_MID, 0, 0);
      mpSecondLabel = lv_label_create(secondCont, NULL);
      lv_label_set_align(mpSecondLabel, LV_LABEL_ALIGN_LEFT);
      lv_obj_add_style(mpSecondLabel, LV_BTN_PART_MAIN, &iconLabelStyle);
    }

    lv_obj_set_user_data(mpContHeader, this);

    mpLabelText = lv_label_create(mpContHeader, NULL);
    
    lv_obj_add_style(mpLabelText, LV_BTN_PART_MAIN, &style_status_label);
    lv_label_set_text(mpLabelText, mLabel.c_str());
    if (!isMultiSensor)
    {
      lv_obj_set_style_local_pad_top(firstCont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 13);
      lv_obj_set_style_local_pad_top(mpLabelText, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 18);
    }

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

  void UISensorComboButton::updateImageWithValue(const std::vector<std::pair<ImagePath, ValueType>> valuePairs)
  {
    ui::resolveAndSetImage(mpImage, std::get<0>(valuePairs.at(0)));
    lv_label_set_text(mpFirstLabel, std::get<1>(valuePairs.at(0)).c_str());

    if (isMultiSensor)// && valuePairs.size() > 1)
    {
     ui::resolveAndSetImage(mpSecondImage, std::get<0>(valuePairs.at(1)));
     lv_label_set_text(mpSecondLabel, std::get<1>(valuePairs.at(1)).c_str());
    }
    mValues = valuePairs;
  }

  void UISensorComboButton::eraseValues()
  {
    mValues.clear();
  }

  void UISensorComboButton::setTextLabel(const std::string label)
  {
    mLabel = label;
    lv_label_set_text(mpLabelText, mLabel.c_str());
  }

  void UISensorComboButton::addTargetAction(ButtonCallback cb)
  {
    mButtonCallback = cb;
  }

  void UISensorComboButton::addTargetLongPressAction(ButtonCallback cb)
  {
    mButtonLongPressCallback = cb;
  }

} // namespace gfx
