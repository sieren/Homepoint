#include <M5Core2.h>

RTC_TimeTypeDef RTCtime;
RTC_TimeTypeDef RTCtime_Now;

char timeStrbuff[64];

void setup()
{
  M5.begin(true,true,true,true);

  RTCtime.Hours = 10;
  RTCtime.Minutes = 30;
  RTCtime.Seconds = 45;

  M5.Lcd.setCursor(0,80);
  M5.Lcd.println("");
  M5.Lcd.println("BtnA:  shutdown, use power button to turn back on");
  M5.Lcd.println("");
  M5.Lcd.println("BtnB:  shutdown, wake up after 5 seconds");
  M5.Lcd.println("");
  M5.Lcd.println("BtnC:  shutdown, wake up at RTC Time 10:30:45");
}

void loop()
{

  M5.update();

  if(M5.BtnA.wasPressed())
  { 
    M5.shutdown();
  }
  if(M5.BtnB.wasPressed())
  {
    M5.shutdown(5);
  }
  if(M5.BtnC.wasPressed())
  {
    M5.shutdown(RTCtime);
  }

  M5.Lcd.setCursor(0,140);
  M5.Rtc.GetTime(&RTCtime_Now);
  sprintf(timeStrbuff,"RTC Time Now is %02d:%02d:%02d",
         RTCtime_Now.Hours,RTCtime_Now.Minutes,RTCtime_Now.Seconds);
  M5.Lcd.println(timeStrbuff);

}
