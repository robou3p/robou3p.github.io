#include <EEPROM.h>
#include <Robot.h>

int addr = 0; //zacetni naslov
float low[6], high[6]; //inicializacija
float error = 0;

void setup() {
  while (!Serial);
  //izbris celotnega EEPROM
  //odkomentiraj samo ce zelis izbrisati vse podatke
  /*
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  */
  Serial.println("Zapuscas setup, pritisni gumb na robotu!");
}

void loop() {
  if(robot.buttonPressed()){
    Serial.println("Odstrani vse ovire pred senzorji!");
    delay(1000);
    //prve prebrane vrednosti zapises v low in high
    robot.distance.readRaw();
    for (int i = 0; i < 6; i++){
      low[i] = robot.distance.sensorsRaw[i];
      high[i] = low[i];
      error = error + low[i];
    }
    if (error == 0.0){ //ce je vsota vseh meritev enaka 0.0 opozori uporabnika in konca test
      Serial.println("Napaka! Meritve v vseh treh smereh so enake 0.00 [m/s2].");
      return;
    }
    delay(5);
    //sedaj preverjas ce so se vrednosti spremenile in jih prepises ce zadostijo pogoju
    //to delas dokler uporabnik ne pritisne gumba na robotu
    Serial.println("Ko so ovire odstranjene, pritisni gumb na robotu za nadaljevanje.");
    Serial.println();
    delay(5);
    while (!robot.buttonPressed()){
      robot.distance.readRaw();
      for (int i = 0; i < 6; i++){
        float sensorReading = robot.distance.sensorsRaw[i];
        if (sensorReading < low[i]){
          low[i] = sensorReading;
        }
        else if (sensorReading > high[i]){
          high[i] = sensorReading;
        }
      }
    }
    delay(5);
    //sedaj iscemo se najvecje vrednosti, ko so senzorji prekriti
    Serial.println("Sedaj postavi dlan pred senzorje!");
    delay(1000);
    Serial.println("Ko končaš, pritisni gumb na robotu za nadaljevanje.");
    delay(5);
    while (!robot.buttonPressed()){
      robot.distance.readRaw();
      for (int i = 0; i < 6; i++){
        float sensorReading = robot.distance.sensorsRaw[i];
        if (sensorReading < low[i]){
          low[i] = sensorReading;
        }
        else if (sensorReading > high[i]){
          high[i] = sensorReading;
        }
      }
    }
    delay(5);
    //izpis rezultatov uporabniku
    Serial.println("/////////////////////////////////////////////////////////");
    Serial.println();
    delay(5);
    //izpis minimalnih prebranih vrednosti low[i]
    Serial.println("MIN: ");
    for (int i = 0; i < 6; i++){
      Serial.print("senzor ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(low[i]);
      EEPROM.put(addr, low[i]); //na mesto addr zapise trenutno vrednost
      addr += sizeof(float); //kazalec za mesto shranjevanja premaknemo naprej za velikost podatka
      delay(5);
    }
    Serial.println();
    delay(5);
    //izpis maksimalnih prebranih vrednosti high[i]
    Serial.println("MAX: ");
    for (int i = 0; i < 6; i++){
      Serial.print("senzor ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(high[i]);
      EEPROM.put(addr, high[i]); //na mesto addr zapise trenutno vrednost
      addr += sizeof(float); //kazalec za mesto shranjevanja premaknemo naprej za velikost podatka
      delay(5);
    }
    Serial.println();
    delay(5);
    Serial.println("/////////////////////////////////////////////////////////");
    Serial.println();
  }

  delay(20);
}
