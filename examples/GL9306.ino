#define GL9306_SERIAL Serial1
#include "GL9306.h"

void setup()
{
  Serial.begin(115200);
  optFlow.begin(23, 27);
}

void loop()
{
  if (optFlow.update())
  {
    optFlow.printValue();
  }
}
