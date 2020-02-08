#pragma once

#include <util/dispatcher.hpp>
#include "esp_event.h"
#include "ESPAsyncWebServer.h"

namespace ctx {
  class AppContext;
}
namespace wifi
{
  class CaptiveServer
  {
    public:
      CaptiveServer(std::shared_ptr<ctx::AppContext> appCtx);
      void startServer();

    private:
      void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
      void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);
      void handleConfigUpdateRequest(AsyncWebServerRequest *request);
      std::unique_ptr<AsyncWebServer> mpServer;
      std::unique_ptr<AsyncWebSocket> mpSocket;
      std::shared_ptr<ctx::AppContext> mpAppContext;
  };

} //namespace web