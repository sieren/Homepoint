#include "Styles.h"


namespace gfx
{
  Styles::Styles()
  {
    lv_style_init(&mainStyle);
    lv_style_set_border_width(&mainStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_inner(&mainStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_left(&mainStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_right(&mainStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_bg_color(&mainStyle, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_text_color(&mainStyle, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_report_style_mod(&mainStyle);
  
    lv_style_copy(&switchBtnContStyle, &mainStyle);
    lv_style_set_bg_color(&switchBtnContStyle, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_bg_color(&switchBtnContStyle, LV_STATE_PRESSED, LV_COLOR_GRAY);
    lv_style_set_bg_color(&switchBtnContStyle, LV_STATE_FOCUSED, LV_COLOR_RED);
    lv_style_set_bg_color(&switchBtnContStyle, LV_STATE_FOCUSED | LV_STATE_PRESSED, lv_color_hex(0xf88));
    lv_style_set_border_width(&switchBtnContStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_text_font(&switchBtnContStyle, LV_STATE_DEFAULT, &lv_font_montserrat_8);
    lv_style_set_pad_inner(&switchBtnContStyle, LV_STATE_DEFAULT, 2);
    lv_style_set_pad_top(&switchBtnContStyle, LV_STATE_DEFAULT, 20);
    lv_style_set_pad_bottom(&switchBtnContStyle, LV_STATE_DEFAULT, 5);

    lv_style_copy(&sensorBtnContStyle, &mainStyle);
    lv_style_set_bg_color(&sensorBtnContStyle, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_bg_color(&sensorBtnContStyle, LV_STATE_PRESSED, LV_COLOR_GRAY);
    lv_style_set_bg_color(&sensorBtnContStyle, LV_STATE_FOCUSED, LV_COLOR_RED);
    lv_style_set_bg_color(&sensorBtnContStyle, LV_STATE_FOCUSED | LV_STATE_PRESSED, lv_color_hex(0xf88));
    lv_style_set_border_width(&sensorBtnContStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_text_font(&sensorBtnContStyle, LV_STATE_DEFAULT, &lv_font_montserrat_12);
    lv_style_set_pad_inner(&sensorBtnContStyle, LV_STATE_DEFAULT, 2);
    lv_style_set_pad_top(&sensorBtnContStyle, LV_STATE_DEFAULT, 18);
    lv_style_set_pad_bottom(&sensorBtnContStyle, LV_STATE_DEFAULT, 5);
  }
}