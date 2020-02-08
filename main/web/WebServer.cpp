#include <AppContext.h>
#include <fs/Filesystem.h>
#include <web/WebServer.h>
#include "SPIFFS.h"
#include "SPIFFSEditor.h"

extern "C" 
{
  #include "esp_log.h"
}

static const char* TAG = "WEB";

namespace web
{

  WebServer::WebServer(std::shared_ptr<ctx::AppContext> appCtx) :
    mpAppContext(appCtx)
  {
    mpServer = std::make_unique<AsyncWebServer>(80);
    using namespace std::placeholders;
    mpServer->addHandler(new SPIFFSEditor(SPIFFS, "admin","admin"));
 
    mpServer->on("/updateConfig.htm", HTTP_GET, std::bind(&WebServer::handleConfigUpdateRequest, this, _1));
  }

  void WebServer::startServer()
  {
    mpServer->begin();
  }

  void WebServer::handleRebootRequest(AsyncWebServerRequest *request)
  {
  // Stub Out
  request->redirect("/index.html");
  }
} // namespace web