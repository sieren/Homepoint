#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>
#include "AdaILI9341Driver.h"

namespace gfx
{
namespace driver
{
  AdaILI9341Driver::AdaILI9341Driver(int CS, int DC, int RST) :
    mDriver(std::make_unique<Adafruit_ILI9341>(CS, DC, RST))
  {};

  void AdaILI9341Driver::begin(int freq)
  {
    mDriver->begin(freq);
  }

  void AdaILI9341Driver::setRotation(int rot)
  {
    mDriver->setRotation(rot);
  }

  void AdaILI9341Driver::drawRect(Frame frame, Color color)
  {
    mDriver->drawRect(frame.position.x, frame.position.y,
      frame.size.width, frame.size.height, color.getColorInt());
  }

  void AdaILI9341Driver::fillRect(Frame frame, Color color)
  {
    mDriver->fillRect(frame.position.x, frame.position.y,
      frame.size.width, frame.size.height, color.getColorInt());
  }

  void AdaILI9341Driver::drawText(Frame frame, Color color, int size, const std::string& text)
  {
    const auto cText = text.c_str();
    mDriver->setCursor(frame.position.x, frame.position.y);
    mDriver->setTextColor(color.getColorInt());
    mDriver->setTextSize(size);
    mDriver->print(cText);
    
  }
} // namespace driver
} // namespace gfx