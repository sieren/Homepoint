#pragma once


namespace mqtt
{
  enum class MQTTSwitchableMode
  {
    DEFAULT,
    ZIGBEE2MQTT // Zigbee2MQTT and HA
  };

  struct MQTTSwitchableDevice
  {
    std::string name;
    std::string getTopic;
    std::string setTopic;

    std::string onValue;
    std::string offValue;

    uint16_t deviceId = 0;
    bool active = false;
    MQTTSwitchableMode mode = MQTTSwitchableMode::ZIGBEE2MQTT;
    std::optional<std::string> iconName;
    std::function<void()> mSetNeedsUpdateCB;
  };
} // namespace mqtt
