#pragma once

#include <util/dispatcher.hpp>
#include "esp_event.h"

#include <optional>

namespace ctx
{
  enum class WifiAssociationState: int
  {
    CONNECTED = 0,
    DISCONNECTED = 1,
    CONNECTING = 2,
    READY = 3
  };

  struct WifiConnectionState
  {
    WifiAssociationState wifiState = WifiAssociationState::DISCONNECTED;
    std::string ipAddr = "N/A";
  };

  using wificallback_t = std::function<void(ctx::WifiAssociationState)>;
  using WifiConnectionStateCB = std::function<void(ctx::WifiConnectionState)>;
  class WifiContext
  {
    public:
      WifiContext();
      void connect(const std::string ssid, const std::string passwd,
        const std::optional<std::string> hostname);
      void disconnect();
      Dispatcher<WifiConnectionState>::CBID registerCallback(WifiConnectionStateCB callback);
      void deleteCallback(Dispatcher<WifiConnectionState>::CBID callback);

      std::string getIpAddrStr() const;
      WifiConnectionState getWifiState();

    private:
      void init();
      void wifiStateChanged(WifiAssociationState state);

      Dispatcher<WifiConnectionState> mWifiStateNotifier;
      std::string mSSID;
      std::string mPassword;
      std::optional<std::string> mHostname;
      WifiConnectionState mLastState;
      wificallback_t mWifiCallback; // Need to store so doesnt go out of scope in C land
  };
} // namespace ctx