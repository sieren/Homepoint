#pragma once

#include "rapidjson/document.h"
#include <util/optional.hpp>

namespace json
{

using JsonObject = rapidjson::Value::ConstObject;
using JsonArray = rapidjson::Value::ConstArray;

template<class T>
struct FunArg : public FunArg<std::decay_t<decltype(&T::operator())>>
{};

template <class Class, class ReturnType, class ArgType>
struct FunArg<ReturnType (Class::*)(ArgType) const>
{
  using Type = ArgType;
};

// Remove const and references
template<class T>
struct ArgDecayHelper
{
  using Type = std::decay_t<T>;
};

// Specialize for const json value refs, otherwise collides with move semantics
template<>
struct ArgDecayHelper<const rapidjson::Value&>
{
  using Type = const rapidjson::Value&;
};

// Derive type from callables argument
// ie. [](std::string str) {} is std::string
template<class T>
using ArgType = typename ArgDecayHelper<typename FunArg<T>::Type>::Type;

template<typename T>
T convertTo(const rapidjson::Value& value, const char* key)
{
  if (!value.Is<T>())
  {
    throw std::runtime_error(std::string("Unable to convert ") + key);
  }

  return value.Get<T>();
}

template<>
JsonArray convertTo<JsonArray>(const rapidjson::Value& value, const char* key)
{
    if (!value.IsArray())
  {
    throw std::runtime_error(std::string("Unable to convert ") + key + " to array");
  }

  return value.GetArray();
}

template<>
JsonObject convertTo<JsonObject>(const rapidjson::Value& value, const char* key)
{
    if (!value.IsObject())
  {
    throw std::runtime_error(std::string("Unable to convert ") + key + " to object");
  }

  return value.GetObject();
}

template<>
double convertTo<double>(const rapidjson::Value& value, const char* key)
{
  if (!value.IsDouble())
  {
    throw std::runtime_error(std::string("Unable to convert ") + key + " to double");
  }

  return value.GetDouble();
}

template<>
bool convertTo<bool>(const rapidjson::Value& value, const char* key)
{
  if (!value.IsBool())
  {
    throw std::runtime_error(std::string("Unable to convert ") + key + " to bool");
  }

  return value.GetBool();
}

template<>
std::string convertTo<std::string>(const rapidjson::Value& value, const char* key)
{
  if (!value.IsString())
  {
    throw std::runtime_error(std::string("Unable to convert ") + key + " to string");
  }

  return std::string(value.GetString());
}

template<typename T>
struct PropertyFinder
{
  template<typename ConsumerFc>
  void read(const rapidjson::Value::ConstObject root, const char* name, ConsumerFc consumer)
  {
    const auto member = root.FindMember(name);
    if (member != root.MemberEnd())
    {
      consumer(convertTo<T>(member->value, name));
    }
  }
};

template<typename T>
struct PropertyFinder<tl::optional<T>>
{
  template<typename ConsumerFc>
  void read(const rapidjson::Value::ConstObject root, const char* name, ConsumerFc consumer)
  {
    const auto member = root.FindMember(name);
    if (member != root.MemberEnd())
    {
      consumer(convertTo<T>(member->value, name));
    }
    else
    {
      consumer(tl::optional<T>{});
    }
  }
};
} // namespace json