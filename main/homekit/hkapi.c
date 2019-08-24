  #define SPIFLASH_BASE_ADDR 0x8c000
  #include <stdio.h>
  #include <string.h>
  #include <esp_wifi.h>
  #include <esp_event_loop.h>
  #include <esp_log.h>
  #include <nvs_flash.h>

  #include <freertos/FreeRTOS.h>
  #include <freertos/task.h>

  #include <homekit/homekit.h>
  #include <homekit/characteristics.h>

homekit_server_config_t config;
homekit_service_t* services[10 + 1];
homekit_service_t** s = services;
homekit_accessory_t *accessories[10];

static int currentPos = 0;

void switch_identify(homekit_value_t _value) {
    // DO NOTHING FOR NOW
}

void hkInitService()
{
    *(s++) = 
      NEW_HOMEKIT_SERVICE(
          ACCESSORY_INFORMATION,
          .id = 1,
          .characteristics=(homekit_characteristic_t*[]) {
              NEW_HOMEKIT_CHARACTERISTIC(NAME, "HomePoint"),
              NEW_HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Homepoint"),
              NEW_HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "131131"),
              NEW_HOMEKIT_CHARACTERISTIC(MODEL, "Button"),
              NEW_HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "0.1"),
              NEW_HOMEKIT_CHARACTERISTIC(IDENTIFY, switch_identify),
              NULL
          },
      );
}

void* memdup(void* data, size_t data_size) {		
     void *result = malloc(data_size);		
     memcpy(result, data, data_size);		
     return result;		
 }

 #define NEW_HOMEKIT_CHARACTERISTIC_CALLBACK(...) \		
     memdup(HOMEKIT_CHARACTERISTIC_CALLBACK(__VA_ARGS__), \		
            sizeof(homekit_characteristic_change_callback_t))

void hkAddDevice(const char* deviceName, homekit_characteristic_t *setterChar)
{
  bool primary = false;
  if (currentPos == 0)
  {
    primary = true;
  }
  *(s++) = NEW_HOMEKIT_SERVICE(
    SWITCH,
    .primary = primary,
    .characteristics=(homekit_characteristic_t*[]) {
      NEW_HOMEKIT_CHARACTERISTIC(NAME, deviceName),
      setterChar,
      NULL
    });
  currentPos += 1;
}

void hkInit(const char* password)
{
  *(s++) = NULL;
  accessories[0] = NEW_HOMEKIT_ACCESSORY(.category=homekit_accessory_category_switch, .services=services);
  accessories[1] = NULL;
  config.accessories = accessories;
  config.password = password;
  homekit_server_init(&config);
}
