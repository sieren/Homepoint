#pragma once

namespace mqtt
{
  
  enum class MQTTSensorType
  {
    // comments are JSON type values
    MQTTValue, // = single value
    MQTTTwoValues, // = temperaturehumidityjson
    MQTTINVALID // Invalid Type
  };

  enum class MQTTSensorDataType
  {
    Raw,
    JSON
  };

  static const std::string MQTTFirstKey = "firstKey";
  static const std::string MQTTSecondKey = "secondKey";

  using DataTypeKey = std::string;
  using JSONValueKey = std::string;
  using Value = std::string;
  using ValueTuple = std::tuple<DataTypeKey, JSONValueKey, Value>;

} // namespace mqtt
