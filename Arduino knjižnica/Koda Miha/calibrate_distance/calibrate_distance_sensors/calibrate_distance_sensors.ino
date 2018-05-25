#include <EEPROM.h>
#include <Robot.h>

void setup() {
  while (!Serial);
  //izbris celotnega EEPROM
  //odkomentiraj samo ce zelis izbrisati vse podatke
  /*
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  */


  int addr = 0; //zacetni naslov
  int low[6], high[6]; //inicializacija
  int error = 0;
  while(!Serial);
  delay(1000);
  Serial.println("Odstrani vse ovire pred senzorji!");
  delay(1000);
  //prve prebrane vrednosti zapises v low in high
  robot.distance.readRaw();
  for (int i = 0; i < 6; i++){
    low[i] = (int)round(robot.distance.sensorsRaw[i]);
    high[i] = low[i];
    error = error + low[i];
  }
  if (error == 0){ //ce je vsota vseh meritev enaka 0.0 opozori uporabnika in konca test
    Serial.println("Napaka! Meritve vseh senzorjev so enake 0.0");
    return;
  }
  delay(5);
  Serial.println("Ko so ovire odstranjene, pritisni funkcijski gumb na robotu za nadaljevanje.");
  Serial.println();
  delay(500);
  //sedaj preverjas ce so se vrednosti spremenile in jih prepises ce zadostijo pogoju
  //to delas dokler uporabnik ne pritisne gumba na robotu
  while (!robot.buttonPressed()){
    robot.distance.readRaw();
    for (int i = 0; i < 6; i++){
      int sensorReading = (int)round(robot.distance.sensorsRaw[i]);
      if (sensorReading < low[i]){
        low[i] = sensorReading;
      }
      else if (sensorReading > high[i]){
        high[i] = sensorReading;
      }
    }
  }
  delay(500);
  //sedaj iscemo se najvecje vrednosti, ko so senzorji prekriti
  Serial.println("Kalibracija traja 10s. Začne in konča se s piskom.");
  Serial.println("Vsakemu senzorju moraš približati steno labirinta kolikor je mogoče!");
  delay(3000);
  Serial.println();

  Serial.println("START kalibracije");
  robot.beep(200,500); //pisk za zacetek kalibracije
  uint32_t tStart = millis();
  do{
    robot.distance.readRaw();
    for (int i = 0; i < 6; i++){
      int sensorReading = (int)round(robot.distance.sensorsRaw[i]);
      if (sensorReading < low[i]){
        low[i] = sensorReading;
      }
      else if (sensorReading > high[i]){
        high[i] = sensorReading;
      }
    }
  } while (millis() < tStart + duration1000ms * 10); //10 s traja kalibracija
  robot.beep(200,500); //pisk za konec kalibracije
  Serial.println("KONEC kalibracije");
  Serial.println();
  delay(1000);

  //izpis rezultatov uporabniku
  Serial.println("REZULTATI");
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
}

void loop() {

}
