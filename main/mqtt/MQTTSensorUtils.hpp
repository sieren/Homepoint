#pragma once

#include "MQTTSensorTypes.hpp"
#include <util/stdextend.hpp>
#include <util/optional.hpp>
#include "rapidjson/document.h"

// todo: remove once part below is fixed upstream
#include <Arduino.h>

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

    const auto& rootObj = document.GetObject();
    float retVal = 0;
    using JsonValue = tl::optional<const rapidjson::Value&>;
    JsonValue result;
    auto jsonSearch = [](rapidjson::Value obj, std::string key)
    {
      auto jsonSearchImpl = [](JsonValue obj, std::string key, auto& jsonSearch_ref) mutable
      {
        if (!obj.has_value())
        {
          return obj;
        }

        for (auto i = obj.value().MemberBegin(); i != obj.value().MemberEnd(); i++)
        {
          if (std::string(i->name.GetString()) == key)
          {
            return tl::make_optional<const rapidjson::Value&>(i->value);
          }
          else if (i->value.IsObject())
          {
            auto jsonRef = tl::make_optional<const rapidjson::Value&>(i->value);
            auto walkValue = jsonSearch_ref(jsonRef, key, jsonSearch_ref);
            if (walkValue.has_value())
            {
              return walkValue;
            }
          }
        }
        return tl::make_optional<const rapidjson::Value&>(tl::nullopt);
      };
      return jsonSearchImpl(obj, key, jsonSearchImpl);
    };

    for (auto i = rootObj.MemberBegin(); i != rootObj.MemberEnd(); i++)
    {
      if (i->value.IsObject())
      {
        result = jsonSearch(i->value.GetObject(), key);
        if (result.has_value())
        {
          break;
        }
      }
      else
      {
        if (std::string(i->name.GetString()) == key)
        {
          result = tl::make_optional<const rapidjson::Value&>(i->value);
        }
      }
    }

    if (!result.has_value())
    {
      return "0";
    }

    auto& foundValue = result.value();

    if (foundValue.IsDouble())
    {
      retVal = static_cast<float>(foundValue.GetDouble());
    }
    else if (foundValue.IsFloat())
    {
      retVal = foundValue.GetFloat();
    }
    else if (foundValue.IsInt())
    {
      retVal = static_cast<float>(foundValue.GetInt());
    }
    else
    {
      return std::string((foundValue.GetString()));
    } 

    std::stringstream stream;
    stream << std::fixed << std::setprecision(1) << retVal;
    std::string s = stream.str();

    return s;
  }

} // namespace util
} // namespace mqtt
