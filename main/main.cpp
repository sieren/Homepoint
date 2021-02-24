
#define ESP_PLATFORM 1
#include <Arduino.h>
#include <SPI.h>
#include <config/Config.h>
#include <fs/ConfigReader.hpp>
#include "AppNewScreen.h"

#include <memory>

extern "C"
{
  #include "freertos/FreeRTOS.h"
  #include "freertos/task.h"
  #include "esp_task_wdt.h"
  #include "nvs_flash.h"
}

#define MAINLOOPCORE 0
TaskHandle_t runLoopHandle = NULL;
bool loopTaskWDTEnabled = false; // Enable if watchdog running

std::shared_ptr<ctx::AppContext> mpAppContext(new ctx::AppContext());
gfx::AppNewScreen mScreen(mpAppContext);

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
    Serial.begin(115200);
    InitializePlatform();
    setupApp();
  }

  void setupApp()
  {
    mScreen.init();
    xTaskCreateUniversal(runLoop, "loopTask", 4096, NULL, 1, &runLoopHandle, MAINLOOPCORE);
    // xTaskCreateUniversal(runLoop, "loopTask", 4096, NULL, 1, &runLoopHandle, MAINLOOPCORE);
    // mScreen.showWarning("Initializing HomePoint");
    try
    {
      mpAppContext->setup();
    }
    catch(const std::exception& e)
    {
      // mScreen.showWarning(e.what());
      // mScreen.registerWifiCallback();
      return;
    }
    if (!mpAppContext->getModel().hasWifiCredentials())
    {
      // mScreen.showWarning("AP: HomePoint-Config, IP: 192.168.99.1");
      return;
    }
    // mScreen.showWarning("Loading Screen");
    // mScreen.setupData();
  }

  void runLoop(void *pvParameters)
  {
    for(;;)
    {
        if (loopTaskWDTEnabled)
        {
          esp_task_wdt_reset();
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
        mScreen.loop();
        // delay(50);
    }
  }
}
