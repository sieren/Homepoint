#include "WifiContext.h"
#include <iomanip>
extern "C" 
{
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"

#include "nvs_flash.h"
#include "tcpip_adapter.h"

#include "lwip/err.h"
#include "lwip/sys.h"
#include <lwip/dns.h>
#include <lwip/netdb.h>
#include <lwip/sockets.h>


static EventGroupHandle_t s_wifi_event_group;
const int WIFI_CONNECTED_BIT = BIT0;
static const char *TAG = "wifi station";

static void event_handler(void* arg, esp_event_base_t event_base, 
                                int32_t event_id, void* event_data)
{
    auto& callback = *reinterpret_cast<ctx::wificallback_t*>(arg);
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
        callback(ctx::WifiAssociationState::READY);
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
        xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        ESP_LOGI(TAG, "retry to connect to the AP");
        callback(ctx::WifiAssociationState::CONNECTING);
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR "\n",
                 IP2STR(&event->ip_info.ip));
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
        callback(ctx::WifiAssociationState::CONNECTED);
    }
}
}


namespace ctx
{
  WifiContext::WifiContext()
  {

  }

  void WifiContext::connect(const std::string ssid, const std::string passwd,
    const std::optional<std::string> hostname)
  {
    mSSID = ssid;
    mPassword = passwd;
    mHostname = hostname;
    init();
  }

  void WifiContext::disconnect()
  {

  }

  Dispatcher<WifiConnectionState>::CBID WifiContext::registerCallback(WifiConnectionStateCB callback)
  {
    return mWifiStateNotifier.addCB(callback);
  }

  void WifiContext::deleteCallback(Dispatcher<WifiConnectionState>::CBID callback)
  {
    mWifiStateNotifier.delCB(callback);
  }

  std::string WifiContext::getIpAddrStr() const
  {
    tcpip_adapter_ip_info_t ipInfo; 

    tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ipInfo);
    char ipAddrStr[30];
	  inet_ntop(AF_INET, &ipInfo.ip.addr, ipAddrStr, sizeof(ipAddrStr));
	  return std::string(ipAddrStr);
  }


  void WifiContext::init()
  {
    s_wifi_event_group = xEventGroupCreate();

    tcpip_adapter_init();

    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    using namespace std::placeholders;
    mWifiCallback = std::bind(&WifiContext::wifiStateChanged, this, _1);

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, &mWifiCallback));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, &mWifiCallback));

    wifi_config_t wifi_config = {};
    strcpy(reinterpret_cast<char*>(wifi_config.sta.ssid), mSSID.c_str());
    strcpy(reinterpret_cast<char*>(wifi_config.sta.password), mPassword.c_str());

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );

    ESP_LOGI(TAG, "wifi_init_sta finished.");
    ESP_LOGI(TAG, "connect to ap SSID:%s password:%s",
             mSSID.c_str(), mPassword.c_str());
  }

  WifiConnectionState WifiContext::getWifiState()
  {
    return mLastState;
  }

  void WifiContext::wifiStateChanged(WifiAssociationState state)
  {
    if (mHostname.has_value() && state == WifiAssociationState::READY) {
      esp_err_t err;
      if ((err = tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_STA, (*mHostname).c_str())) != ESP_OK)
      {
        ESP_LOGI(TAG, "Unable to set hostname.");
      }
    }
    if (state == WifiAssociationState::CONNECTED) {
      mLastState.ipAddr = getIpAddrStr();
    }
    mLastState.wifiState = state;
    mWifiStateNotifier.broadcast(mLastState);
  }

} // namespace ctx