#include <Robot.h>

int sPrag = 45;
int zPrag = 60;
bool lOK, dOK = false;

void setup() {
  while(!Serial);
  delay(100);
  Serial.println("Odstrani vse ovire pred senzorji!");
  delay(3000);
  robot.distance.readRaw();

  int S0 = (int)round(robot.distance.sensorsRaw[0] / 10);
  int S1 = (int)round(robot.distance.sensorsRaw[1] / 10);
  int S2 = (int)round(robot.distance.sensorsRaw[2] / 10);
  int S3 = (int)round(robot.distance.sensorsRaw[3] / 10);
  int S4 = (int)round(robot.distance.sensorsRaw[4] / 10);
  int S5 = (int)round(robot.distance.sensorsRaw[5] / 10);

  //preverjanje, ali senzor brez ovir daje pravilno vrednost (manj od sPrag)
  if (S0 > sPrag){
    Serial.println("Senzor S0 zaznava oviro. Če pred njim ni ovire, je senzor pokvarjen.");
    delay(5);
  }
  if (S1 > sPrag){
    Serial.println("Senzor S1 zaznava oviro. Če pred njim ni ovire, je senzor pokvarjen.");
    delay(5);
  }
  if (S2 > sPrag){
    Serial.println("Senzor S2 zaznava oviro. Če pred njim ni ovire, je senzor pokvarjen.");
    delay(5);
  }
  if (S3 > sPrag){
    Serial.println("Senzor S3 zaznava oviro. Če pred njim ni ovire, je senzor pokvarjen.");
    delay(5);
  }
  if (S4 > sPrag){
    Serial.println("Senzor S4 zaznava oviro. Če pred njim ni ovire, je senzor pokvarjen.");
    delay(5);
  }
  if (S5 > sPrag){
    Serial.println("Senzor S5 zaznava oviro. Če pred njim ni ovire, je senzor pokvarjen.");
    delay(5);
  }
  Serial.println("//////////////////////////////////");
  Serial.println();
}

void loop() {

  robot.distance.readRaw();

  int S0 = (int)round(robot.distance.sensorsRaw[0] / 10);
  int S1 = (int)round(robot.distance.sensorsRaw[1] / 10);
  int S2 = (int)round(robot.distance.sensorsRaw[2] / 10);
  int S3 = (int)round(robot.distance.sensorsRaw[3] / 10);
  int S4 = (int)round(robot.distance.sensorsRaw[4] / 10);
  int S5 = (int)round(robot.distance.sensorsRaw[5] / 10);
/*
  //Izpis prebranih vrednosti na posameznem senzorju
  for (int i = 0; i < 6; i++){
    Serial.print((int)round(robot.distance.sensorsRaw[i] / 10));
    Serial.print('\t');
  }
  Serial.println();
*/
  
  delay(5);
  if (!lOK){
    Serial.println("Postavi dlan pred LEVE tri senzorje!");
    delay(5);
    bool dlanL = S0 > zPrag && S1 > zPrag && S2 > zPrag;
    if (dlanL){
      Serial.println("Levi senzorji delujejo.");
      lOK = true;
      delay(5);
    }
  }

  delay(5);
  if (!dOK){
    Serial.println("Postavi dlan pred DESNE tri senzorje!");
    delay(5);
    bool dlanD = S3 > zPrag && S4 > zPrag && S5 > zPrag;
    if (dlanD){
      Serial.println("Desni senzorji delujejo.");
      dOK = true;
      delay(5);
    }
  }
  Serial.println("//////////////////////////////////");
  Serial.println();
  if (robot.buttonPressed()) while(1); //ustavi delovanje
  delay(3000);
}
