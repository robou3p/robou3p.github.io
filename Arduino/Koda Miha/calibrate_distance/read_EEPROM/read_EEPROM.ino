#include <EEPROM.h>

void setup() {

  float f = 0.00f;   //Variable to store data read from EEPROM.
  int address = 0; //EEPROM address to start reading from
  while (!Serial);

  for (int i = 0; i < 12; i++){
    //Get the float data from the EEPROM at position 'address'
    EEPROM.get(address, f);
    Serial.println(f, 3);    //This may print 'ovf, nan' if the data inside the EEPROM is not a valid float.
    address += sizeof(float);
  }
}

void loop() {
  /* Empty loop */
}
