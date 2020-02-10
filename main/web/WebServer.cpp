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

  WebServer::WebServer(std::shared_ptr<ctx::AppContext> appCtx, WebCredentials webCredentials) :
    mpAppContext(appCtx),
    mCredentials(webCredentials)
  {
    mpServer = std::make_unique<AsyncWebServer>(80);
    using namespace std::placeholders;
    mpServer->addHandler(new SPIFFSEditor(SPIFFS, "admin","admin"));
    const auto username = std::get<0>(mCredentials);
    const auto password = std::get<1>(mCredentials);
    mpServer->serveStatic("/", SPIFFS, "/").setDefaultFile("/index.htm").setAuthentication(username.c_str(), password.c_str());
 
    mpServer->on("/updateConfig.htm", HTTP_GET, std::bind(&WebServer::handleRebootRequest, this, _1));
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