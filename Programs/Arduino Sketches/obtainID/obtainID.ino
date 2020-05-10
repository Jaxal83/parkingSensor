// ArduinoUniqueIDSerialUSB.ino
//
// Example shows the UniqueID on the SerialUSB Monitor.
// Use this to find out the Unique ID of the Feather
/*
	ID 1
		UniqueID: 59 89 3A 4A 50 52 51 51 32 2E 31 20 FF 0D 17 41 
		UniqueID: 32 2E 31 20 FF 0D 17 41 
		
	ID 2
		UniqueID: 15 98 F4 AD 50 52 51 51 32 2E 31 20 FF 0D 13 3B 
		UniqueID: 32 2E 31 20 FF 0D 13 3B 
	
	ID 3
		UniqueID: 8D 06 32 B7 50 52 51 51 32 2E 31 20 FF 0E 26 06 
		UniqueID: 32 2E 31 20 FF 0E 26 06 

	
	ID 4
		UniqueID: 5F 4E 3E 8C 50 52 51 51 32 2E 31 20 FF 0E 15 2B 
		UniqueID: 32 2E 31 20 FF 0E 15 2B 

	
	ID 5
		UniqueID: 2F 82 3E 60 50 52 51 51 32 2E 31 20 FF 09 2F 2C 
		UniqueID: 32 2E 31 20 FF 09 2F 2C 

	ID 6
		UniqueID: 5F
	
	ID 7
		UniqueID: D9
*/

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
