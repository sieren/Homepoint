#pragma once

#include "Adafruit_ILI9341.h"
#include "Color.hpp"

#include "uiposition.hpp"
#include <memory>

class Adafruit_ILI9341;

namespace gfx
{
namespace driver
{
  class AdaILI9341Driver
  {
    public:
      AdaILI9341Driver(int CS, int DC, int RST);
     // AdaILI9341Driver(const AdaILI9341Driver& o) = delete;

      void begin(int freq = 0);
      void setRotation(int rot);

      void drawRect(Frame frame, Color color);
      void fillRect(Frame frame, Color color);
      void drawText(Frame frame, Color color, int size, const std::string& text);

      void commit() { /* no op */ };

    private:
      std::unique_ptr<Adafruit_ILI9341> mDriver;
      
  };
} // namespace driver
} // namespace gfx