#pragma once

#include <string>

namespace homekit
{
  struct HKConfig
  {
    std::string password;
    bool isEnabled = false;
  };
} // namespace homekit
