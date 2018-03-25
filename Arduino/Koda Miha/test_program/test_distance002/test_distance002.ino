#include <Robot.h>

int low[6], high[6]; //inicializacija

void setup() {
  while(!Serial);
  delay(100);
}

void loop() {
  Serial.println("Odstrani vse ovire pred senzorji!");
  delay(1000);
  
  //prve prebrane vrednosti zapises v low in high
  robot.distance.readRaw();
  for (int i = 0; i < 6; i++){
    low[i] = (int)round(robot.distance.sensorsRaw[i]);
    high[i] = low[i];
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
      int sensorReading = (int)round(robot.distance.sensorsRaw[i]);
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
      int sensorReading = (int)round(robot.distance.sensorsRaw[i]);
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
  delay(5);
  //izpis zaporednih stevilk senzorjev
  Serial.print("ŠT.: ");
  for (int i = 0; i < 6; i++){
    Serial.print(i);
    Serial.print('\t');
    delay(5);
  }
  Serial.println();
  delay(5);
  //izpis minimalnih prebranih vrednosti low[i]
  Serial.print("MIN: ");
  for (int i = 0; i < 6; i++){
    Serial.print(low[i]);
    Serial.print('\t');
    delay(5);
  }
  Serial.println();
  delay(5);
  //izpis maksimalnih prebranih vrednosti high[i]
  Serial.print("MAX: ");
  for (int i = 0; i < 6; i++){
    Serial.print(high[i]);
    Serial.print('\t');
    delay(5);
  }
  Serial.println();
  delay(5);
  Serial.println("/////////////////////////////////////////////////////////");
  Serial.println();
  delay(5);
}
