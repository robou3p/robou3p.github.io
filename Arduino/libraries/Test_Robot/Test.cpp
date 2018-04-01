/*
  Test.cpp - Library for testing the robot.
  Created by Miha Klokocovnik, March 3, 2018.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Robot.h"
#include "Test.h"
#include "Motor.h"
#include "MPU9250.h"
#include "Line.h"
#include "Distance.h"
#include <avr/interrupt.h>
#include <math.h>
#include "Wire.h"

Test test = Test();

/*
  Izpise napetost baterije v [V].
*/
void Test::battery(){
  while (!Serial); //nic se ne zgodi dokler ne odpres serial monitorja
  if (robot.battery() == 0.0){ //ce je napetost baterije enaka 0.0 opozori uporabnika in konca test
    Serial.println("Napaka! Baterija ni vstavljena.");
    return;
  }
  Serial.print("Baterija ima ");
  Serial.print(robot.battery());
  Serial.println("V napetosti.");
}

/*
  Izpise ali levi motor deluje pravilno ali ne.
*/
void Test::leftMotor(){
  while (!Serial); //nic se ne zgodi dokler ne odpres serial monitorja
  Serial.println("Za začetek testa LEVEGA MOTORJA, dvignite robota v zrak in pritisnite gumb (tisti, ki NI zraven USB kabla)!");
  Serial.println();
  while (!robot.buttonPressed()); //caka na pritisk gumba za nadaljevanje programa
  Serial.println("/////////////////////////");
  Serial.println("Če želite prekiniti test, pritisnite gumb.");
  delay(2000); //zakasnitev, da debouncing
  float U = 4.0;
  int rez = 0;
  while (!rez){
    if (robot.buttonPressed()){
      break;
    }
    robot.motor[LEFT].setVoltage(U);
    delay(1000);
    if (robot.motor[LEFT].getSpeed() == 0.0){
      Serial.println("Enkoderji ne delujejo. Funkcija robot.motor[LEFT].getSpeed() vrača 0.00");
      delay(5);
      break;
    }
    if (robot.motor[LEFT].getSpeed() > 5.0){
      Serial.println("Levi motor se pravilno vrti naprej.");
      robot.motor[LEFT].setVoltage(-U);
      delay(1000);
      if (robot.motor[LEFT].getSpeed() < -5.0){
        Serial.println("Levi motor se pravilno vrti nazaj.");
        rez = 1;
      }
    }
  }
  robot.motor[LEFT].setVoltage(0);
  Serial.println();
}

/*
  Izpise ali desni motor deluje pravilno ali ne.
*/
void Test::rightMotor(){
  while (!Serial); //nic se ne zgodi dokler ne odpres serial monitorja
  Serial.println("Za začetek testa DESNEGA MOTORJA, dvignite robota v zrak in pritisnite gumb (tisti, ki NI zraven USB kabla)!");
  Serial.println();
  while (!robot.buttonPressed()); //caka na pritisk gumba za nadaljevanje programa
  Serial.println("/////////////////////////");
  Serial.println("Če želite prekiniti test, pritisnite gumb.");
  delay(2000); //zakasnitev, da debouncing
  float U = 4.0;
  int rez = 0;
  while (!rez){
    if (robot.buttonPressed()){
      break;
    }
    robot.motor[RIGHT].setVoltage(U);
    delay(1000);
    if (robot.motor[RIGHT].getSpeed() == 0.0){
      Serial.println("Enkoderji ne delujejo. Funkcija robot.motor[RIGHT].getSpeed() vrača 0.00");
      delay(5);
      break;
    }
    if (robot.motor[RIGHT].getSpeed() > 5.0){
      Serial.println("Desni motor se pravilno vrti naprej.");
      robot.motor[RIGHT].setVoltage(-U);
      delay(1000);
      if (robot.motor[RIGHT].getSpeed() < -5.0){
        Serial.println("Desni motor se pravilno vrti nazaj.");
        rez = 1;
      }
    }
  }
  robot.motor[RIGHT].setVoltage(0);
  Serial.println();
}

/*
  Vklopi brencac za 5 sekund.
*/
void Test::buzzer(){
  while (!Serial); //nic se ne zgodi dokler ne odpres serial monitorja
  Serial.println("Za začetek testa BRENČAČA pritisnite gumb na desni strani robota (tisti, ki NI zraven USB kabla)!");
  Serial.println();
  while (!robot.buttonPressed()); //caka na pritisk gumba za nadaljevanje programa
  Serial.println("Robot bo piskal 5 sekund. Če želite prekiniti test, pritisnite gumb.");
  delay(2000); //zakasnitev, da debouncing
  int16_t frequency = 500; //frekvenca brencaca
  int16_t timeBuzzerON = 100; //koliko casa je prizgan brencac
  int16_t error = 0; //ali je uporabnik pritisnil gumb za prekinitev
  uint32_t tStart1 = millis();
  uint32_t tStart2;
  do {
    robot.beep(frequency, timeBuzzerON); //prizge brencac
    tStart2 = millis();
    do{
      if (robot.buttonPressed()){ //ce je pritisnjen gumb zakljuci test
        error = 1;
        Serial.println("Prekinili ste test!");
        break;
      }
    } while (millis() < tStart2 + duration1000ms/10); //100ms je tiho brencac
    if (error) break;
    if (robot.buttonPressed()){ //ce je pritisnjen gumb zakljuci test
        Serial.println("Prekinili ste test!");
        break;
      }
  } while (millis() < tStart1 + duration1000ms*5); //brencac deluje 5s
  Serial.println();
}

/*
  Enkrat izpiše temperaturo v enoti [C].
*/
void Test::temp(){
  robot.imu.readSensor();
  float T = robot.imu.getTemperature_C();
  if (T == 0.0){ //ce je temperatura enaka 0.0 opozori uporabnika in konca test
    Serial.println("Napaka! Senzor za temperaturo ne deluje.");
    return;
  }
  Serial.print("Temperatura je: ");
  Serial.print(T);
  Serial.println(" C");
}

/*
  Enkrat izpiše magnetno polje v X,Y,Z smeri v enoti [uT].
*/
void Test::mag(){
  robot.imu.readSensor();
  float magX = robot.imu.getMagX_uT();
  float magY = robot.imu.getMagY_uT();
  float magZ = robot.imu.getMagZ_uT();
  if (magX + magY + magZ == 0.0){ //ce je vsota v vseh 3eh smereh enaka 0.0 opozori uporabnika in konca test
    Serial.println("Napaka! Meritve v vseh treh smereh so enake 0.00 [uT].");
    return;
  }
  Serial.print("Magnetno polje v uT je: ");
  Serial.print("X=");
  Serial.print((int)floor(magX));
  Serial.print("  Y=");
  Serial.print((int)floor(magY));
  Serial.print("  Z=");
  Serial.println((int)floor(magZ));
}

/*
  Preveri ali je pospešek v X,Y,Z smeri večji od praga. Enote so [m/s2].
*/
void Test::accel(){
  while (!Serial); //nic se ne zgodi dokler ne odpres serial monitorja
  Serial.println("Za začetek testa POSPEŠKOMETRA pritisnite gumb na desni strani robota (tisti, ki NI zraven USB kabla)!");
  Serial.println();
  while (!robot.buttonPressed()); //caka na pritisk gumba za nadaljevanje programa
  Serial.println("/////////////////////////");
  Serial.println("Robota primite v roke in ga premikajte v vseh 3eh smereh, dokler ne izpiše Test končan.");
  Serial.println("Če želite prekiniti test, pritisnite gumb.");
  delay(3000); //zakasnitev, da debouncing

  int tmpX, tmpY, tmpZ = 0;
  float prag = 11.0; //nastavi prag obcutljivosti [m/s2], ki ga mora senzor preseci, da opravi test
  int16_t error = 0; //ali je uporabnik pritisnil gumb za prekinitev
  uint32_t tStart1;

  while (!robot.buttonPressed()) {
    robot.imu.readSensor();
    float accelX = robot.imu.getAccelX_mss();
    float accelY = robot.imu.getAccelY_mss();
    float accelZ = robot.imu.getAccelZ_mss();

    if (accelX + accelY + accelZ == 0.0){ //ce je vsota v vseh 3eh smereh enaka 0.0 opozori uporabnika in konca test
      Serial.println("Napaka! Meritve v vseh treh smereh so enake 0.00 [m/s2].");
      break;
    }

    Serial.print("X=");
    Serial.print(accelX);
    Serial.print("  Y=");
    Serial.print(accelY);
    Serial.print("  Z=");
    Serial.println(accelZ);

    if (abs(accelX) > prag) tmpX = 1; //preveri ce je zaznano gibanje mocnejse od paga
    if (abs(accelY) > prag) tmpY = 1;
    if (abs(accelZ) > prag) tmpZ = 1;
    if (tmpX && tmpY && tmpZ){ //ce je zaznano gibanje v vseh 3eh smereh opozori uporabnika in konca test
      Serial.println("Pospeškometer deluje!  Test zaključen.");
      break;
    }
    tStart1 = millis();
    do{
      if (robot.buttonPressed()){ //ce je pritisnjen gumb zakljuci test
        error = 1;
        Serial.println("Prekinili ste test!");
        break;
      }
    } while (millis() < tStart1 + duration1000ms/5); //delay za 200ms zaradi stabilnosti delovanja
    if (error) break;
  }
  Serial.println();
}

/*
  Preveri ali je premik v X,Y,Z smeri večji od praga. Enote so [rad/s].
*/
void Test::gyro(){
  while (!Serial); //nic se ne zgodi dokler ne odpres serial monitorja
  Serial.println("Za začetek testa ŽIROSKOPA pritisnite gumb na desni strani robota (tisti, ki NI zraven USB kabla)!");
  Serial.println("Če so meritve v vseh treh smereh ob premikanju enake 0.00, senzor ne deluje pravilno.");
  Serial.println();
  while (!robot.buttonPressed()); //caka na pritisk gumba za nadaljevanje programa
  Serial.println("/////////////////////////");
  Serial.println("Robota primite v roke in ga premikajte v vseh 3eh smereh, dokler ne izpiše Test končan.");
  Serial.println("Če želite prekiniti test, pritisnite gumb.");
  delay(3000); //zakasnitev, da debouncing

  int tmpX, tmpY, tmpZ = 0;
  float prag = 1.0; //nastavi prag obcutljivosti [rad/s], ki ga mora senzor preseci, da opravi test
  int16_t error = 0; //ali je uporabnik pritisnil gumb za prekinitev
  uint32_t tStart1;

  while(!robot.buttonPressed()){
    robot.imu.readSensor();
    float gyroX = robot.imu.getGyroX_rads();
    float gyroY = robot.imu.getGyroY_rads();
    float gyroZ = robot.imu.getGyroZ_rads();

    Serial.print("X=");
    Serial.print(gyroX);
    Serial.print("  Y=");
    Serial.print(gyroY);
    Serial.print("  Z=");
    Serial.println(gyroZ);

    if (abs(gyroX) > prag) tmpX = 1; //preveri ce je zaznano gibanje mocnejse od paga
    if (abs(gyroY) > prag) tmpY = 1;
    if (abs(gyroZ) > prag) tmpZ = 1;
    if (tmpX && tmpY && tmpZ){ //ce je zaznano gibanje v vseh 3eh smereh opozori uporabnika in konca test
      Serial.println("Žiroskop deluje!  Test zaključen.");
      break;
    }
    tStart1 = millis();
    do{
      if (robot.buttonPressed()){ //ce je pritisnjen gumb zakljuci test
        error = 1;
        Serial.println("Prekinili ste test!");
        break;
      }
    } while (millis() < tStart1 + duration1000ms/5); //delay za 200ms
    if (error) break;
  }
  Serial.println();
}

/*
  Izpise MIN in MAX prebrane vrednosti posameznih senzorjev blizine.
*/
void Test::distance(){
  int low[6], high[6]; //inicializacija
  int error = 0;
  while(!Serial);
  delay(100);
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
  Serial.println();
  delay(5);
  //izpis minimalnih prebranih vrednosti low[i]
  Serial.println("MIN: ");
  for (int i = 0; i < 6; i++){
    Serial.print("senzor ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(low[i]);
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
    delay(5);
  }
  Serial.println();
  delay(5);
  Serial.println("/////////////////////////////////////////////////////////");
  Serial.println();
  delay(5);
}

/*
  Uporabniku sporoci, ali senzorji za crto delujejo pravilno.
*/
void Test::line(){
  while(!Serial);
  delay(100);
  //kalibracija senzorjev
  Serial.println("Kalibracija traja 5s. Začne in konča se s piskom.");
  Serial.println("Pritisni gumb za začetek.");
  Serial.println();
  while(!robot.buttonPressed());
  delay(500);
  Serial.println("START kalibracije");
  robot.beep(200,500); //pisk za konec kalibracije
  robot.line.calibrate(5000);
  robot.beep(200,500); //pisk za konec kalibracije
  Serial.println("KONEC kalibracije");
  delay(500);
  Serial.println("Pritisni gumb za nadaljevanje testa.");
  while(!robot.buttonPressed());
  delay(1000); //debouncing
  Serial.println();
  Serial.println("Črto daj pod SREDINO robota.");
  while(!robot.buttonPressed()){
    if (abs(robot.line.getPosition() * 100) < 1.0){
      Serial.println("--- OK ---");
      robot.beep(500,300);
      break;
    }
  }
  delay(1000);
  Serial.println();
  Serial.println("Črto daj pod LEVO stran robota.");
  while(!robot.buttonPressed()){
    if ((robot.line.getPosition() * 100) < -2.5){
      Serial.println("--- OK ---");
      robot.beep(500,300);
      break;
    }
  }
  delay(1000);
  Serial.println();
  Serial.println("Črto daj pod DESNO stran robota.");
  while(!robot.buttonPressed()){
    if ((robot.line.getPosition() * 100) > 2.5){
      Serial.println("--- OK ---");
      robot.beep(500,300);
      break;
    }
  }
  delay(10);
  Serial.println();
  Serial.println("Test zaključen!!!");
  Serial.println();
  delay(10);
}
