#pragma once

namespace mqtt
{
  
  enum class MQTTSensorType
  {
    // comments are JSON type values
    MQTTTemperature, // = temperature
    MQTTHumidity, // = humidity
    MQTTTemperatureHumidity, // = temperaturehumidityjson
    MQTTVOC, // = voc
    MQTTINVALID // Invalid Type
  };

  enum class MQTTSensorDataType
  {
    Raw,
    JSON
  };

  inline static const std::string MQTTSensorTypeJSONKey(MQTTSensorType sensorType)
  {
    const std::map<MQTTSensorType, const std::string> sensorTypes {
      {MQTTSensorType::MQTTHumidity, "humidityKey"},
      {MQTTSensorType::MQTTTemperature, "temperatureKey"},
      {MQTTSensorType::MQTTTemperatureHumidity, "temperaturehumidity"},
      {MQTTSensorType::MQTTVOC, "vocKey"}
    };
    auto it = sensorTypes.find(sensorType);
    return it == sensorTypes.end() ? "Out of range" : it->second;
  }

  static const std::string MQTTHumidityKeyJSON = "humidityKey";
  static const std::string MQTTTemperatureKeyJSON = "temperatureKey";
  static const std::string MQTTVOCKeyJSON = "vocKey";

  using DataTypeKey = std::string;
  using JSONValueKey = std::string;
  using Value = std::string;
  using ValueTuple = std::tuple<DataTypeKey, JSONValueKey, Value>;
}