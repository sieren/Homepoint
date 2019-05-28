
#define ESP_PLATFORM 1
#include <Arduino.h>
#include <SPI.h>
#include <config/Config.h>
#include <fs/ConfigReader.hpp>
#include "AppScreen.hpp"

#include <memory>

extern "C"
{
  #include "freertos/FreeRTOS.h"
  #include "freertos/task.h"
  #include "esp_task_wdt.h"
  #include "nvs_flash.h"
}

#define MAINLOOPCORE 1
TaskHandle_t runLoopHandle = NULL;
bool loopTaskWDTEnabled = false; // Enable if watchdog running

std::shared_ptr<ctx::AppContext> mpAppContext(new ctx::AppContext());
gfx::AppScreen<ScreenDriver, NavigationDriver> mScreen(mpAppContext);

extern "C" 
{
  void runLoop(void *pvParameters);
  void setupApp();

  void app_main()
  {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    initArduino();
    setupApp();
  }

  void setupApp()
  {
    mScreen.setupScreen();
    xTaskCreateUniversal(runLoop, "loopTask", 8192, NULL, 1, &runLoopHandle, MAINLOOPCORE);
    mScreen.showWarning("Initializing HomePoint");
    try
    {
      mpAppContext->setup();
    }
    catch(const std::exception& e)
    {
      mScreen.showWarning(e.what());
      return;
    }
    mScreen.showWarning("Loading Screen");
    const auto wifiCredentials = fs::ConfigReader::getWifiCredentials();
    mpAppContext->getWifiContext().connect(std::get<0>(wifiCredentials), std::get<1>(wifiCredentials));
    mScreen.setupData();
    mpAppContext->getMQTTConnection()->connect();
  }

  void runLoop(void *pvParameters)
  {
    for(;;) {
        if(loopTaskWDTEnabled){
            esp_task_wdt_reset();
        }
        mScreen.draw();
        delay(50);
    }
  }
}
