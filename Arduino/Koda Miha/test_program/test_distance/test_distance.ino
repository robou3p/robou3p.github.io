#include <Robot.h>

void setup() {
  while(!Serial);
}

void loop() {
  int sPrag = 10;
  int zPrag = 50;
  bool lOK, dOK = false;
  robot.distance.readRaw();
  
  int S0 = (int)round(robot.distance.sensorsRaw[0] / 10);
  int S1 = (int)round(robot.distance.sensorsRaw[1] / 10);
  int S2 = (int)round(robot.distance.sensorsRaw[2] / 10);
  int S3 = (int)round(robot.distance.sensorsRaw[3] / 10);
  int S4 = (int)round(robot.distance.sensorsRaw[4] / 10);
  int S5 = (int)round(robot.distance.sensorsRaw[5] / 10);
  
  for (int i = 0; i < 6; i++){
    Serial.print((int)round(robot.distance.sensorsRaw[i] / 10));
    Serial.print('\t');
  }
  Serial.println();

  bool ovira = S0 > sPrag && S1 > sPrag && S2 > sPrag && S3 > sPrag && S4 > sPrag && S5 > sPrag;
  if (ovira){
    Serial.println("Odstrani ovire pred senzorji!");
  }
  if (!lOK){
    Serial.println("Postavi dlan pred LEVE tri senzorje!");
    bool dlanL = S0 > zPrag && S1 > zPrag && S2 > zPrag;
    if (dlanL){
      Serial.println("Levi senzorji delujejo.");
      lOK = true;
    }
  }

  if (!dOK){
    Serial.println("Postavi dlan pred DESNE tri senzorje!");
    bool dlanD = S3 > zPrag && S4 > zPrag && S5 > zPrag;
    if (dlanD){
      Serial.println("Desni senzorji delujejo.");
      dOK = true;
    }
  }

  delay(1000);
}
