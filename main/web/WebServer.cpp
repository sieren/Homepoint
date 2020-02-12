#include <AppContext.h>
#include <fs/Filesystem.h>
#include <web/WebServer.h>
#include "SPIFFS.h"
#include "SPIFFSEditor.h"

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
  }

  void WebServer::startServer()
  {
    mpServer->begin();
  }

  void WebServer::handleRebootRequest(AsyncWebServerRequest *request)
  {
    ESP.restart();
  }
} // namespace web