#pragma once

#include "MQTTSensorTypes.hpp"
#include <util/stdextend.hpp>
#include "rapidjson/document.h"

// todo: remove once part below is fixed upstream
#include <Arduino.h>

#include <algorithm>
#include <iomanip>
#include <optional>
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
    if (typeName == "oneValue") { return MQTTSensorType::MQTTValue; }
    if (typeName == "twoValues") { return MQTTSensorType::MQTTTwoValues; }
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
    using JsonValueRef = std::reference_wrapper<const rapidjson::Value>;
    using JsonValue = std::optional<JsonValueRef>;
    JsonValue result;
    auto jsonSearch = [](const rapidjson::Value obj, std::string key)
    {
      auto jsonSearchImpl = [](JsonValue obj, std::string key, auto& jsonSearch_ref) mutable
      {
        if (!obj.has_value())
        {
          return obj;
        }

        auto &objRef = obj.value().get();
        for (auto i = objRef.MemberBegin(); i != objRef.MemberEnd(); i++)
        {
          if (std::string(i->name.GetString()) == key)
          {
            return std::optional<JsonValueRef>{i->value};
          }
          else if (i->value.IsObject())
          {
            auto jsonRef = std::optional<JsonValueRef>{i->value};
            auto walkValue = jsonSearch_ref(jsonRef, key, jsonSearch_ref);
            if (walkValue.has_value())
            {
              return walkValue;
            }
          }
        }
        return obj;
      };
      auto objOpt = std::make_optional<JsonValueRef>(std::ref(obj));
      return jsonSearchImpl(objOpt, key, jsonSearchImpl);
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
          result = std::make_optional<JsonValueRef>(i->value);
        }
      }
    }

    if (!result.has_value())
    {
      return "0";
    }

    auto& foundValue = result.value().get();

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
