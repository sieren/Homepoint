#pragma once

//#include <SPI.h>
#define GFXFF 1
#include <M5Core2.h>
#include "ui/Color.hpp"
#include "ui/UIPosition.hpp"
#include <memory>
#include <string>
#include <vector>

namespace gfx
{
namespace driver
{
  class TFTM5STACKESPI
  {
    public:
      TFTM5STACKESPI(int width, int height) :
        mSprite(&mDriver),
        mWidth(width),
        mHeight(height)
      {};

      void begin(int freq = 0)
      {
        mDriver.init();
        mSprite.setColorDepth(8);
      }

      void createSprite(Frame frame, Color background)
      {
        mSprite.createSprite(frame.size.width, frame.size.height);
        mSprite.fillSprite(background.getColorInt());
      }

      void pushSprite(Point pos)
      {
        mSprite.pushSprite(pos.x, pos.y);
      }

      void deleteSprite()
      {
        mSprite.deleteSprite();
      }

      void setRotation(int rot)
      {
        mDriver.setRotation(rot);
      }

      void setDisplayInverted(bool inverted)
      {
        mDriver.invertDisplay(inverted);
      }

      void drawRect(Frame frame, Color color)
      {
        mSprite.drawRect(frame.position.x, frame.position.y, frame.size.width, frame.size.height, color.getColorInt());
      }

      void drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, Color lineColor)
      {
        mSprite.drawLine(x0, y0, x1, y1, lineColor.getColorInt());
      }

      void drawDirectLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, Color lineColor)
      {
        mDriver.drawLine(x0, y0, x1, y1, lineColor.getColorInt());
      }

      void drawJpg(const std::string filePath, const Point position)
      {
        mSprite.drawJpgFile(SPIFFS, filePath.c_str(), position.x, position.y);
      }

      void drawJpg(const uint8_t* arrayname, size_t imageSize, const Point position)
      {
        printf("Size: %i\r\n", imageSize);
        mSprite.drawJpg(arrayname, imageSize, position.x, position.y);
      }

      void setTextColor(Color textColor, Color backgroundColor)
      {
        mSprite.setTextColor(textColor.getColorInt(), backgroundColor.getColorInt());
      }

      void drawText(Frame frame, int size, const std::string& text, uint8_t gfxHandle = 1)
      {
        mSprite.drawString(text.c_str(), frame.position.x, frame.position.y);
      }

      void drawText(Frame frame, Color color, int size, const std::string& text, uint8_t gfxHandle = 1)
      {
        mSprite.setTextColor(color.getColorInt());
        mSprite.drawString(text.c_str(), frame.position.x, frame.position.y, gfxHandle);
      }
    
      void setFreeFont(const GFXfont *font)
      {
        mDriver.setFreeFont(font);
      }

      auto getTextWidth(const std::string textString) -> int16_t
      {
        return mSprite.textWidth(textString.c_str());
      }

      void setCursor(int x, int y)
      {
        mSprite.setCursor(x, y);
      }

      void println(const std::string text)
      {
        mSprite.println(text.c_str());
      }
      
      void loadFont(const std::string fontName)
      {
        if (mLoadedFont == fontName)
        {
          return;
        }
        mSprite.loadFont(fontName.c_str());
        mLoadedFont = fontName;
      }

      void unloadFont()
      {
        mSprite.unloadFont();
        mLoadedFont = "";
      }

      void commit()
      {
      }
    
      M5Display* getDriverRef()
      {
        return &mDriver;
      }

      void writeCommand(uint8_t command)
      {
        mDriver.writecommand(command);
      }

      void setSleep(bool on)
      {
        if (on)
        {
          mDriver.sleep();
        }
        else
        {
          mDriver.wakeup();
        }
      }

    private:
      M5Display mDriver;
      TFT_eSprite mSprite;
      int mWidth;
      int mHeight;
      std::string mLoadedFont;
      
  };
} // namespace driver
} // namespace gfx
