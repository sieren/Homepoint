
#define ESP_PLATFORM 1
#include <Arduino.h>
#include <SPI.h>
#include <memory>
#include <config/Config.h>
#include <fs/ConfigReader.hpp>
#include "AppScreen.hpp"


std::shared_ptr<ctx::AppContext> mpAppContext(new ctx::AppContext());
gfx::AppScreen<ScreenDriver, NavigationDriver> mScreen(mpAppContext);

void setup() {
  Serial.begin(115200); 
  mpAppContext->setup();
  const auto wifiCredentials = fs::ConfigReader::getWifiCredentials();
  mpAppContext->getWifiContext().connect(std::get<0>(wifiCredentials), std::get<1>(wifiCredentials));

  mScreen.setup();

  mpAppContext->getMQTTConnection()->connect();
}

void loop() {
  mScreen.draw();
  delay(50);
}
