#pragma once

#include <util/dispatcher.hpp>
#include "esp_event.h"

namespace ctx
{
  enum class WifiAssociationState: int
  {
    CONNECTED = 0,
    DISCONNECTED = 1,
    CONNECTING = 2
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
      void connect(const std::string ssid, const std::string passwd);
      void disconnect();
      void registerCallback(WifiConnectionStateCB callback);

      std::string getIpAddrStr() const;
      WifiConnectionState getWifiState();

    private:
      void init();
      void wifiStateChanged(WifiAssociationState state);

      Dispatcher<WifiConnectionState> mWifiStateNotifier;
      std::string mSSID;
      std::string mPassword;
      wificallback_t mWifiCallback; // Need to store so doesnt go out of scope in C land
  };
} // namespace ctx