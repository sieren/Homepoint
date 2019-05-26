#pragma once

#include "MQTTGroup.hpp"
#include "MQTTSensorUtils.hpp"
#include "MQTTSensorTypes.hpp"

#include <vector>

namespace mqtt
{
  struct MQTTSensorDevice
  {
    std::string name;
    std::string getTopic;

    std::string value;
    std::vector<ValueTuple> mappedValues;

    std::string getTemperature()
    {
      if (dataType == MQTTSensorDataType::JSON)
      {
        return util::FindValue(mappedValues, MQTTTemperatureKeyJSON);
      }
      return value;
    }

    std::string getHumidity()
    {
      if (dataType == MQTTSensorDataType::JSON)
      {
        return util::FindValue(mappedValues, MQTTHumidityKeyJSON);
      }
      return value;
    }
    
    std::string getVOC()
    {
      if (dataType == MQTTSensorDataType::JSON)
      {
        return util::FindValue(mappedValues, MQTTVOCKeyJSON);
      }
      return value;
    }

    void setValue(const std::string topicData)
    {
      if (dataType == MQTTSensorDataType::JSON)
      {
        for (auto& dev : mappedValues)
        {
          const auto key = std::get<1>(dev);
          const auto val = util::JsonToValues(topicData, key);
          std::get<2>(dev) = val;
        }
      }
      else
      {
        value = topicData;
      }
    }

    int deviceId = 0;
    MQTTSensorType sensorType;
    MQTTSensorDataType dataType;
  };

  struct MQTTSensorGroup : public MQTTGroup
  {
    GroupDevices<MQTTSensorDevice> mSensorDevices;
  };
} // namespace mqtt
