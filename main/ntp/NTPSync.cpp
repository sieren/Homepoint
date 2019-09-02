#include <ntp/NTPSync.h>
#include <util/timer.hpp>

extern "C"
{
  #include <string.h>
  #include <time.h>
  #include <sys/time.h>
  #include "freertos/FreeRTOS.h"
  #include "freertos/task.h"
  #include "freertos/event_groups.h"
  #include "esp_system.h"
  #include "esp_event.h"
  #include "esp_log.h"
  #include "esp_attr.h"
  #include "esp_sleep.h"
  #include "nvs_flash.h"
  #include "esp_sntp.h"
}

static const char *TAG = "NTP";
static void obtain_time(void);
static void initialize_sntp(void);

namespace ntp
{

  NTPSync::NTPSync(const std::string timeZone) :
    mTimeZone(timeZone)
  {

  }

  void NTPSync::syncTime()
  {
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);

    obtain_time();
    // update 'now' variable with current time
    time(&now);

    char strftime_buf[64];
    setenv("TZ", mTimeZone.c_str(), 1);
    tzset();
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);

    const auto syncInterval = std::chrono::hours(12);
    Timer::async(syncInterval, [&](){
      syncTime();
    });
  }
} // namespace ntp

static void obtain_time(void)
{
    initialize_sntp();

    // wait for time to be set
    time_t now = 0;
    struct tm timeinfo = { };
    int retry = 0;
    const int retry_count = 10;
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    time(&now);
    localtime_r(&now, &timeinfo);
}

static void initialize_sntp(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    char serverName[] = "pool.ntp.org"; // Should be const, blame the sntp API.
    sntp_setservername(0, serverName);
    sntp_init();
}
