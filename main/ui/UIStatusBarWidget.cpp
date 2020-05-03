#include "UIStatusBarWidget.h"

#include <config/Config.h>
#include <config/Icon.hpp>
#include <ntp/NTPSync.h>
#include <functional>

namespace gfx
{

  UIStatusBarWidget::UIStatusBarWidget(ScreenDriver* screenptr, Frame frame, uint16_t tag) :
    UIWidget(screenptr, frame, tag)
    {
      mLastUpdate = std::chrono::system_clock::now() - std::chrono::seconds(StatusBarUpdateInterval);
    }
  
  void UIStatusBarWidget::wifiChanged(ctx::WifiConnectionState connectionState)
  {
    using namespace ctx;
    const auto status = connectionState.wifiState;

    switch(status)
    {
      case WifiAssociationState::CONNECTING:
        mIpAddressLabel = "CONNECTING...";
        mWifiImage = "wifi_off";
        break;
      case WifiAssociationState::CONNECTED:
        mIpAddressLabel = connectionState.ipAddr;
        mWifiImage = "wifi_on";
        break;
      case WifiAssociationState::DISCONNECTED:
        mIpAddressLabel = "DISCONNECTED";
        mWifiImage = "wifi_off";
        break;
      case WifiAssociationState::READY:
        mIpAddressLabel = "READY";
        mWifiImage = "wifi_off";
        break;
    }
    mNeedsRedraw = true;
  }

  void UIStatusBarWidget::mqttConnectionChanged(mqtt::MQTTConnectionStatus connectionState)
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
    mNeedsRedraw = true;
  }

  void UIStatusBarWidget::draw()
  {
    auto now = std::chrono::system_clock::now();
    if (std::chrono::duration_cast<std::chrono::seconds>(now - mLastUpdate).count() > StatusBarUpdateInterval)
    {
      mLastUpdate = std::chrono::system_clock::now();
      mNeedsRedraw = true;
      mIsWifiDisplayed = !mIsWifiDisplayed;
    }
    if (mNeedsRedraw == false) 
    {
      return;
    }
    mpScreen->unloadFont();
    mpScreen->createSprite(mFrame, mBackgroundColor);

    auto mWifiImageFrame = Frame();
    mWifiImageFrame.position = {0, 0, 0};
    mpScreen->drawJpg(util::GetIconFilePath(mWifiImage), mWifiImageFrame.position);

    auto mMqttImageFrame = Frame();
    mMqttImageFrame.position = {mFrame.size.width - 25, 0, 0};
    mpScreen->drawJpg(util::GetIconFilePath(mMqttImage), mMqttImageFrame.position);

    auto textLabelFrame = mFrame;
    const auto timeIpAddrLabel = mIsWifiDisplayed ? mIpAddressLabel : ntp::util::GetCurrentTime();
    const auto textLabel = mTextLabel == "" ? timeIpAddrLabel : mTextLabel;
    const auto textWidth = mpScreen->getTextWidth(textLabel.c_str());
    const auto centerPoint = mFrame.getCenterPoint();
    textLabelFrame.position.x = centerPoint.x - textWidth / 2;
    textLabelFrame.position.y = centerPoint.y - 4; 
    mpScreen->drawText(textLabelFrame, mTextColor, 1, textLabel);
    mNeedsRedraw = false;
    mpScreen->pushSprite(mFrame.position);
    mpScreen->deleteSprite();
  }

  void UIStatusBarWidget::setTextLabel(const std::string label)
  {
    mTextLabel = label;
  }

  void UIStatusBarWidget::setTextColor(const Color color)
  {
    mTextColor = color;
  }

  void UIStatusBarWidget::registerCallback(ctx::WifiContext* ctx)
  {
    ctx->registerCallback(std::bind(&UIStatusBarWidget::wifiChanged, this, std::placeholders::_1));
  }

} // namespace gfx
