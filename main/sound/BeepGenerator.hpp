#pragma once

namespace sound
{
  struct BeepGenerator
  {
    static void ShortBeep()
    {
      ledcWriteTone(0, 2000);
      delay(10);
      ledcWriteTone(0, 0);
      digitalWrite(0, 0);
    }
  };
} // namespace sound
