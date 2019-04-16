#pragma once

#include <iterator>
#include <sstream>
#include <string> 

namespace estd
{
  // ESP32 STL is missing std::string::to_string
  template <typename T>
  std::string to_string(T value)
  {
      std::ostringstream os ;
      os << value ;
      return os.str() ;
  }
} // namespace estd

namespace util
{
  // Safely advance until end()
  template <typename Iterator>
  size_t safe_advance(Iterator& it, Iterator const& end, size_t n)
  {
    size_t i = 0;
    for (; i != n && it != end; ++i, ++it);
    return n - i;
  }
}
