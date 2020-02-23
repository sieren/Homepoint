#pragma once

#include <util/dispatcher.hpp>
#include <model/Model.hpp>
#include "esp_event.h"
#include "ESPAsyncWebServer.h"

namespace ctx {
  class AppContext;
}
namespace web
{
  class WebServer
  {
    public:
      WebServer(std::shared_ptr<ctx::AppContext> appCtx, WebCredentials webCredentials);
      void startServer();

    private:
      void handleRebootRequest(AsyncWebServerRequest *request);
      void handleReloadRequest(AsyncWebServerRequest *request);
      String processor(const String& var);
      std::unique_ptr<AsyncWebServer> mpServer;
      std::unique_ptr<AsyncWebSocket> mpSocket;
      std::shared_ptr<ctx::AppContext> mpAppContext;
      WebCredentials mCredentials;
  };

} //namespace web