#include <AppContext.h>
#include <fs/Filesystem.h>
#include <wifi/CaptiveServer.h>
#include <WiFi.h>
#include <DNSServer.h>
#include "SPIFFS.h"
#include "SPIFFSEditor.h"

extern "C" 
{
  #include "esp_log.h"
}

static const char* TAG = "WEB";

namespace wifi
{

  const byte DNS_PORT = 53;
  IPAddress apIP(192, 168, 99, 1);
  DNSServer dnsServer;

  CaptiveServer::CaptiveServer(std::shared_ptr<ctx::AppContext> appCtx) :
    mpAppContext(appCtx)
  {
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP("HomePoint-Config");
    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
      delay(1000);
    }
    dnsServer.start(DNS_PORT, "*", apIP);
    mpServer = std::make_unique<AsyncWebServer>(80);
    using namespace std::placeholders;
    mpServer->serveStatic("/", SPIFFS, "/").setDefaultFile("/captive.html");
    mpServer->on("/updateConfig.htm", HTTP_POST, std::bind(&CaptiveServer::handleConfigUpdateRequest, this, _1));
    mpServer->begin();
  }

  void CaptiveServer::startServer()
  {
    mpServer->begin();
  }

  void CaptiveServer::handleConfigUpdateRequest(AsyncWebServerRequest *request)
  {
    std::string ssid = "";
    std::string ssidPassword = "";
    std::string webLogin = "";
    std::string webPassword = "";

    if (request->hasParam("ssid", true))
    {
      AsyncWebParameter* paramUN = request->getParam("ssid", true);
      ssid = std::string(paramUN->value().c_str());
    }

    if (request->hasParam("ssidPassword", true))
    {
      AsyncWebParameter* paramUN = request->getParam("ssidPassword", true);
      ssidPassword = std::string(paramUN->value().c_str());
    }

    if (request->hasParam("loginName", true))
    {
      AsyncWebParameter* paramUN = request->getParam("loginName", true);
      webLogin = std::string(paramUN->value().c_str());
    }

    if (request->hasParam("loginPassword", true))
    {
      AsyncWebParameter* paramUN = request->getParam("loginPassword", true);
      webPassword = std::string(paramUN->value().c_str());
    }

    ESP_LOGI(TAG, "Updating Configuration");
    const auto credentials = std::make_tuple(ssid, ssidPassword);
    mpAppContext->setFirstLaunch(credentials, webLogin, webPassword);
    request->redirect("/index.html");
  }
} // namespace web
