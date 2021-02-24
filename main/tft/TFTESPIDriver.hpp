#pragma once

//#include <SPI.h>
#define GFXFF 1
#include "TFT_eSPI.h"
#include "TFT_eFEX.h"
#include "ui/Color.hpp"
#include "ui/UIPosition.hpp"
#include <memory>
#include <string>
#include <vector>

#include "lv_conf.h"
#include "lvgl.h"


namespace gfx
{
namespace driver
{
  static TFT_eSPI tft_espi(TFT_WIDTH,TFT_HEIGHT); 
  class TFTESPI
  {
    public:
      TFTESPI() :
        mSprite(&tft_espi),
        mEfx(&tft_espi),
        mWidth(320),
        mHeight(270)
      {};

      void begin()
      {

      }
  
      void init(int rotation, bool isInverted)
      {
     //   tft_espi.initDMA(); 

        // tft_espi.begin();
        // tft_espi.setSwapBytes(true);
        // tft_espi.setRotation(rotation);
        // tft_espi.invertDisplay(isInverted);
      }

      static void flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p)
      {
        size_t len = lv_area_get_size(area);
        tft_espi.startWrite();                                      /* Start new TFT transaction */
        tft_espi.setWindow(area->x1, area->y1, area->x2, area->y2); /* set the working window */
        #ifdef USE_DMA_TO_TFT
        tft_espi.pushPixelsDMA((uint16_t *)color_p, len); /* Write words at once */
        #else
        tft_espi.pushPixels((uint16_t *)color_p, len); /* Write words at once */
        #endif
        tft_espi.endWrite(); /* terminate TFT transaction */
  Serial.println("Flushed");
        /* Tell lvgl that flushing is done */
        lv_disp_flush_ready(disp);
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
        tft_espi.setRotation(rot);
      }

      void setDisplayInverted(bool inverted)
      {
        tft_espi.invertDisplay(inverted);
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
        tft_espi.drawLine(x0, y0, x1, y1, lineColor.getColorInt());
      }

      void drawJpg(const std::string filePath, const Point position)
      {
        mEfx.drawJpeg(filePath.c_str(), position.x, position.y, &mSprite);
      }

      void drawJpg(const uint8_t* arrayname, size_t imageSize, const Point position)
      {
        printf("Size: %i\r\n", imageSize);
        mEfx.drawJpeg(arrayname, imageSize, position.x, position.y, &mSprite);
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
        tft_espi.setFreeFont(font);
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
    
      TFT_eSPI* getTouchDriverRef()
      {
        return &tft_espi;
      }

      void writeCommand(uint8_t command)
      {
        tft_espi.writecommand(command);
      }

    private:
      TFT_eSprite mSprite;
      TFT_eFEX mEfx;
      int mWidth;
      int mHeight;
      std::string mLoadedFont;
      
  };
} // namespace driver
} // namespace gfx
