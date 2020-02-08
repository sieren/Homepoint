#pragma once

#include <util/dispatcher.hpp>
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
      WebServer(std::shared_ptr<ctx::AppContext> appCtx);
      void startServer();

    private:
      void handleRebootRequest(AsyncWebServerRequest *request);
      String processor(const String& var);
      std::unique_ptr<AsyncWebServer> mpServer;
      std::unique_ptr<AsyncWebSocket> mpSocket;
      std::shared_ptr<ctx::AppContext> mpAppContext;
  };

} //namespace web