#pragma once

#include "MQTTSensorTypes.hpp"
#include <util/stdextend.hpp>
#include "rapidjson/document.h"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <tuple>

extern "C"
{
  #include "esp_log.h"
}

namespace mqtt
{
namespace util
{
  inline static auto GetSensorType(const std::string typeName) -> MQTTSensorType
  {
    // map JSON Keys to Enum
    if (typeName == "temperature") { return MQTTSensorType::MQTTTemperature; }
    if (typeName == "humidity") { return MQTTSensorType::MQTTHumidity; }
    if (typeName == "temperaturehumidity") { return MQTTSensorType::MQTTTemperatureHumidity; }
    if (typeName == "voc") { return MQTTSensorType::MQTTVOC; }
    return MQTTSensorType::MQTTINVALID;
  }

  inline static auto FindValue(std::vector<ValueTuple>& vec, const DataTypeKey dataType) -> std::string
  {
    auto it = std::find_if(vec.begin(), vec.end(),
      [&dataType](const ValueTuple& e)
      {
        return std::get<0>(e) == dataType;
      });

    if (it == vec.end()) 
    {
      return "";
    }

    return std::get<2>(*it);
  }

  inline auto JsonToValues(const std::string data, std::string key) -> std::string
  {
    using namespace rapidjson;
    Document document;
    ParseResult res = document.Parse<0>(data.c_str());
    if (!res)
    {
      ESP_LOGI("MQTT", "Unable to parse JSON");
      return "0";
    }
    float retVal;
    if (document[key.c_str()].IsDouble())
    {
      retVal = static_cast<float>(document[key.c_str()].GetDouble());
    }
    else if (document[key.c_str()].IsFloat())
    {
      retVal = document[key.c_str()].GetFloat();
    }
    else if (document[key.c_str()].IsInt())
    {
      retVal = static_cast<float>(document[key.c_str()].GetInt());
    }
    else
    {
      return std::string((document[key.c_str()].GetString()));
    } 
    std::stringstream stream;
    stream << std::fixed << std::setprecision(1) << retVal;
    std::string s = stream.str();
    return s;
  }

} // namespace util
} // namespace mqtt
