#pragma once

//#include <SPI.h>
#include "ILI9341_SPI.h"
#include "MiniGrafx.h" // General graphic library
#include "Color.hpp"

#include "uiposition.hpp"
#include <memory>

    static uint16_t palette[] = {ILI9341_BLACK,     //  0
              ILI9341_WHITE,     //  1
              ILI9341_NAVY,      //  2
              ILI9341_DARKCYAN,  //  3
              ILI9341_DARKGREEN, //  4
              ILI9341_MAROON,    //  5
              ILI9341_PURPLE,    //  6
              ILI9341_OLIVE,     //  7
              ILI9341_LIGHTGREY, //  8
              ILI9341_DARKGREY,  //  9
              ILI9341_BLUE,      // 10
              ILI9341_GREEN,     // 11
              ILI9341_CYAN,      // 12
              ILI9341_RED,       // 13
              ILI9341_MAGENTA,   // 14
              0xFD80};           // 15

namespace gfx
{
namespace driver
{
  class MiniGrafxDriver
  {
    public:
      MiniGrafxDriver(int CS, int DC, int RST) :
        mDriver(CS, DC, RST),
        mMiniGrafx(&mDriver, 4, palette, 240, 320)
      {};
     // AdaILI9341Driver(const AdaILI9341Driver& o) = delete;

      void begin(int freq = 0)
      {
        mMiniGrafx.init();
        mMiniGrafx.fillBuffer(0);
        mMiniGrafx.commit();
      }

      void setRotation(int rot)
      {
        mMiniGrafx.setRotation(rot);
      }

      void drawRect(Frame frame, Color color)
      {
        mMiniGrafx.setColor(palette[rand()%15]);
        mMiniGrafx.drawRect(frame.position.x, frame.position.y, frame.size.width, frame.size.height);
      }

      void fillRect(Frame frame, Color color)
      {
        mMiniGrafx.setColor(palette[rand()%15]);
        mMiniGrafx.fillRect(frame.position.x, frame.position.y, frame.size.width, frame.size.height);
        commit();
      }

      void drawText(Frame frame, Color color, int size, const std::string& text)
      {
        mMiniGrafx.setColor(1);
        mMiniGrafx.drawStringMaxWidth(frame.position.x, frame.position.y, frame.size.width, text.c_str());
      }

      void commit()
      {
        mMiniGrafx.commit();
      }
    
    private:
      ILI9341_SPI mDriver;
      MiniGrafx mMiniGrafx;
      
  };
} // namespace driver
} // namespace gfx