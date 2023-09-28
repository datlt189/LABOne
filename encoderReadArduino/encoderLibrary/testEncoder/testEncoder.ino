#include "encoderRead.h"

#define encoderA 16
#define encoderB 17

Encoder encoder(encoderA, encoderB, 4);

void getfunc()
{
  encoder.readEncoder();
}

void setup() 
{
  Serial.begin(115200);
  encoder.init();
  encoder.setup(getfunc);
}

void loop() 
{
  int pulseCount = encoder.getPulse();
  Serial.println(pulseCount);
  delay(100); // Add a delay to control the printing rate (adjust as needed)
}