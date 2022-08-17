#define GL9306_SERIAL Serial1
#include "GL9306.h"
GL9306 optFlow(23, 27);
void setup()
{
  Serial.begin(115200);
}

void loop()
{
  if (optFlow.available())
  {
    optFlow.printValue();
  }
}
