#include "UIStatusBar.h"

#include <config/Config.h>
#include <config/Icon.hpp>
#include <ntp/NTPSync.h>
#include <functional>

namespace gfx
{

  UIStatusBar::UIStatusBar(lv_obj_t *parent) :
    mpParent(parent)
  {
    mLastUpdate = std::chrono::system_clock::now() - std::chrono::seconds(StatusBarUpdateInterval);
    mpContHeader = lv_cont_create(mpParent, NULL);
    lv_cont_set_fit2(mpContHeader, LV_FIT_MAX, LV_FIT_TIGHT);
    lv_obj_set_height(mpContHeader, 10);
    lv_cont_set_layout(mpContHeader, LV_LAYOUT_ROW_TOP);
    lv_obj_align(mpContHeader, mpParent, LV_ALIGN_IN_TOP_MID, 0, 0);

    lv_obj_t *cont_header_left = lv_cont_create(mpContHeader, NULL);
    lv_cont_set_fit(cont_header_left, LV_FIT_TIGHT);
    lv_cont_set_layout(cont_header_left, LV_LAYOUT_ROW_MID);
   // lv_obj_set_event_cb(cont_header_left, display_mainmenu_event);
    lv_obj_align(cont_header_left, mpContHeader, LV_ALIGN_IN_TOP_LEFT, 10, 0);

    lv_obj_t *cont_header_right = lv_cont_create(mpContHeader, NULL);
    lv_cont_set_fit(cont_header_right, LV_FIT_TIGHT);
    lv_cont_set_layout(cont_header_right, LV_LAYOUT_ROW_MID);
   // lv_obj_set_event_cb(cont_header_left, display_mainmenu_event);
    lv_obj_align(cont_header_right, mpContHeader, LV_ALIGN_IN_TOP_RIGHT, 100, 0);

    static lv_style_t lv_style1;
    lv_style_set_bg_color(&lv_style1, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_bg_color(&lv_style1, LV_STATE_PRESSED, LV_COLOR_GRAY);
    lv_style_set_bg_color(&lv_style1, LV_STATE_FOCUSED, LV_COLOR_RED);
    lv_style_set_bg_color(&lv_style1, LV_STATE_FOCUSED | LV_STATE_PRESSED, lv_color_hex(0xf88));
    lv_style_set_border_width(&lv_style1, LV_STATE_DEFAULT, 0);
    lv_style_set_text_font(&lv_style1, LV_STATE_DEFAULT, &lv_font_montserrat_8);
    lv_style_set_pad_inner(&lv_style1, LV_STATE_DEFAULT, 2);
    lv_style_set_pad_top(&lv_style1, LV_STATE_DEFAULT, 1);
    lv_style_set_pad_bottom(&lv_style1, LV_STATE_DEFAULT, 5);
    // lv_style_set_pad_left(&lv_style1, LV_STATE_DEFAULT, 0);
    // lv_style_set_pad_right(&lv_style1, LV_STATE_DEFAULT, 0);

    static lv_style_t innerStyle;
    lv_style_copy(&innerStyle, &lv_style1);
    lv_style_set_pad_top(&innerStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_bottom(&innerStyle, LV_STATE_DEFAULT, 1);
    lv_style_set_pad_left(&innerStyle, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_right(&innerStyle, LV_STATE_DEFAULT, 0);
    lv_obj_add_style(mpContHeader, LV_OBJ_PART_MAIN, &lv_style1);
    lv_obj_add_style(cont_header_left, LV_OBJ_PART_MAIN, &innerStyle);
    lv_obj_add_style(cont_header_right, LV_OBJ_PART_MAIN, &innerStyle);

    mpStatusLabel = lv_label_create(cont_header_left, NULL);
    static lv_style_t style_status_label;
    lv_style_copy(&style_status_label, &innerStyle);
    lv_obj_add_style(mpStatusLabel, LV_BTN_PART_MAIN, &style_status_label);
    lv_label_set_text(mpStatusLabel, mIpAddressLabel.c_str());

    mpStatusSymbol = lv_label_create(cont_header_right, NULL);
    lv_obj_add_style(mpStatusSymbol, LV_BTN_PART_MAIN, &style_status_label);
    lv_label_set_text(mpStatusSymbol, mWifiImage.c_str());
  }
  
  void UIStatusBar::wifiChanged(ctx::WifiConnectionState connectionState)
  {
    using namespace ctx;
    const auto status = connectionState.wifiState;

    switch(status)
    {
      case WifiAssociationState::CONNECTING:
        mIpAddressLabel = "CONNECTING...";
        mWifiImage = LV_SYMBOL_WARNING;
        break;
      case WifiAssociationState::CONNECTED:
        mIpAddressLabel = connectionState.ipAddr;
        mWifiImage = LV_SYMBOL_WIFI;
        break;
      case WifiAssociationState::DISCONNECTED:
        mIpAddressLabel = "DISCONNECTED";
        mWifiImage = LV_SYMBOL_WARNING;
        break;
      case WifiAssociationState::READY:
        mIpAddressLabel = "READY";
        mWifiImage = LV_SYMBOL_WARNING;
        break;
    }
    lv_label_set_text(mpStatusLabel, mIpAddressLabel.c_str());
    lv_label_set_text(mpStatusSymbol, mWifiImage.c_str());
  }

  void UIStatusBar::mqttConnectionChanged(mqtt::MQTTConnectionStatus connectionState)
  {
    switch (connectionState)
    {
      case mqtt::MQTTConnectionStatus::CONNECTED:
        mMqttImage = "mqtt_on";
        break;
      case mqtt::MQTTConnectionStatus::DISCONNECTED:
        mMqttImage = "mqtt_off";
        break;
      case mqtt::MQTTConnectionStatus::CONNECTING:
        mMqttImage = "mqtt_off";
        break;
    }
  }


  void UIStatusBar::setTextLabel(const std::string label)
  {
    mTextLabel = label;
  }

  void UIStatusBar::setTextColor(const Color color)
  {
    mTextColor = color;
  }

  void UIStatusBar::registerCallback(ctx::WifiContext* ctx)
  {
    ctx->registerCallback(std::bind(&UIStatusBar::wifiChanged, this, std::placeholders::_1));
  }

} // namespace gfx
