#include <AppContext.h>
#include <fs/Filesystem.h>
#include <web/WebServer.h>
#include "SPIFFS.h"
#include "SPIFFSEditor.h"
#include <Update.h>

extern "C"
{
  #include "esp_log.h"
}

static const char* TAG = "WEBSERVER";

namespace web
{

  WebServer::WebServer(std::shared_ptr<ctx::AppContext> appCtx, WebCredentials webCredentials) :
    mpAppContext(appCtx),
    mCredentials(webCredentials)
  {
    mpServer = std::make_unique<AsyncWebServer>(80);
    using namespace std::placeholders;
    const auto username = std::get<0>(mCredentials);
    const auto password = std::get<1>(mCredentials);
    mpServer->serveStatic("/", SPIFFS, "/web").setAuthentication(username.c_str(), password.c_str());
    mpServer->addHandler(new SPIFFSEditor(SPIFFS, username.c_str(), password.c_str()));
 
    mpServer->on("/reboot.htm", HTTP_POST, std::bind(&WebServer::handleRebootRequest, this, _1));
    mpServer->on("/reload.htm", HTTP_POST, std::bind(&WebServer::handleReloadRequest, this, _1));
    mpServer->on("/otaUpdate", HTTP_POST,
      [](AsyncWebServerRequest *request) {},
      [&](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data,
        size_t len, bool final)
      {
        handleOTA(request, filename, index, data, len, final);
      }
    );
  }

  void WebServer::handleOTA(AsyncWebServerRequest *request, const String& filename,
    size_t index, uint8_t *data, size_t len, bool final)
  {
    if (!index)
    {
      ESP_LOGI(TAG, "Begin Update");
      if (!Update.begin(UPDATE_SIZE_UNKNOWN, U_FLASH))
      {
        Update.printError(Serial);
      }
    }

    if (Update.write(data, len) != len)
    {
      Update.printError(Serial);
    }

    if (final)
    {
      AsyncWebServerResponse *response = request->beginResponse(302,
        "text/plain", "Please wait while the device reboots");
      response->addHeader("Refresh", "20");
      response->addHeader("Location", "/");
      request->send(response);
      if (!Update.end(true))
      {
        Update.printError(Serial);
      }
      else
      {
        ESP_LOGI(TAG, "Update Complete");
        Serial.flush();
        ESP.restart();
      }
    }
  }

    void WebServer::startServer()
    {
      mpServer->begin();
    }

    void WebServer::handleRebootRequest(AsyncWebServerRequest *request)
    {
      ESP.restart();
    }

    void WebServer::handleReloadRequest(AsyncWebServerRequest *request)
    {
      mpAppContext->reload();
    }
} // namespace web
