#define USE_SERIAL Serial
#include "GL9306OpticalFlow.h"

GL9306 optFlow(&Serial2, 15, 2);

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  if (optFlow.available())
    optFlow.printValue();
}
