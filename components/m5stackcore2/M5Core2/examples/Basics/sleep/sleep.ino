#include <M5Core2.h>

void setup()
{
  M5.begin();

  M5.Lcd.setTextFont(2);

  Serial.println("Light / Deep Sleep Test.");
  M5.Lcd.println("Light / Deep Sleep Test.");

  Serial.println("Going to light sleep for 5 seconds.");
  M5.Lcd.println("Going to light sleep for 5 seconds.");
  delay(2500);

  M5.Axp.LightSleep(SLEEP_SEC(5));

  Serial.println("Wakeup from light sleep.");
  M5.Lcd.println("Wakeup from light sleep.");
  delay(1000);

  Serial.println("Going to deep sleep for 5 seconds.");
  M5.Lcd.println("Going to deep sleep for 5 seconds.");
  delay(2500);

  M5.Axp.DeepSleep(SLEEP_SEC(5));
  // Never reached
}

void loop()
{

}
