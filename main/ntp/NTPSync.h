#pragma once

#include <chrono> 
#include <ctime>
#include <sstream>
#include <iomanip>
#include <string>

namespace ntp
{
  class NTPSync
  {
    public:
      NTPSync(const std::string timeZone);
      void syncTime();
    
    private:
      const std::string mTimeZone;

  };

namespace util
{
  inline static const std::string GetCurrentTime()
  {
      auto now = std::chrono::system_clock::now();
      auto in_time_t = std::chrono::system_clock::to_time_t(now);

      std::stringstream ss;
      ss << std::put_time(std::localtime(&in_time_t), "%H:%M");
      return ss.str();
  }
} // namespace util
} // namespace time