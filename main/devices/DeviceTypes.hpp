#pragma once

#include <mqtt/MQTTGroup.hpp>
#include <homekit/HKDevice.hpp>
#include "mpark/variant.hpp"

using DeviceVariants = mpark::variant<MQTTSwitchGroupPtr, MQTTSensorGroupPtr, HKDevicePtr, MQTTGroupBasePtr>;
