// ArduinoUniqueIDSerialUSB.ino
//
// Example shows the UniqueID on the SerialUSB Monitor.
//

#include <ArduinoUniqueID.h>

void setup()
{
  Serial.begin(9600);
  while (!Serial);
  UniqueIDdump(Serial);
  UniqueID8dump(Serial);
}

void loop()
{
}
