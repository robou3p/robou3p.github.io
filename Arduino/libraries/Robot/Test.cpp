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

/*
  Izpise napetost baterije v [V].
*/
void Test::battery(){
  while (!Serial); //nic se ne zgodi dokler ne odpres serial monitorja
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
  float U = 2.0;
  int rez = 0;
  while (!rez){
    if (robot.buttonPressed()){
      break;
    }
    robot.motor[LEFT].setVoltage(U);
    delay(1000);
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
  float U = 2.0;
  int rez = 0;
  while (!rez){
    if (robot.buttonPressed()){
      break;
    }
    robot.motor[RIGHT].setVoltage(U);
    delay(1000);
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
}

/*
  Vklopi brencac za 5 sekund.
*/
void Test::buzzer(){
  while (!Serial); //nic se ne zgodi dokler ne odpres serial monitorja
  Serial.println("Za začetek testa BRENČAČA pritisnite gumb na desni strani robota (tisti, ki NI zraven USB kabla)!");
  Serial.println();
  while (!robot.buttonPressed()); //caka na pritisk gumba za nadaljevanje programa
  Serial.println("/////////////////////////");
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
}

/*
  Enkrat izpiše temperaturo v enoti [C].
*/
void Test::temp(){
  robot.imu.readSensor();
  float T = robot.imu.getTemperature_C();
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
  delay(2000); //zakasnitev, da debouncing

  int tmpX, tmpY, tmpZ = 0;
  float prag = 11.0; //nastavi prag obcutljivosti [m/s2], ki ga mora senzor preseci, da opravi test
  int16_t error = 0; //ali je uporabnik pritisnil gumb za prekinitev
  uint32_t tStart1;

  while (!robot.buttonPressed()) {
    robot.imu.readSensor();
    float accelX = robot.imu.getAccelX_mss();
    float accelY = robot.imu.getAccelY_mss();
    float accelZ = robot.imu.getAccelZ_mss();

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
    } while (millis() < tStart1 + duration1000ms/5); //delay za 200ms
    if (error) break;
  }
}

/*
  Preveri ali je premik v X,Y,Z smeri večji od praga. Enote so [rad/s].
*/
void Test::gyro(){
  while (!Serial); //nic se ne zgodi dokler ne odpres serial monitorja
  Serial.println("Za začetek testa ŽIROSKOPA pritisnite gumb na desni strani robota (tisti, ki NI zraven USB kabla)!");
  Serial.println();
  while (!robot.buttonPressed()); //caka na pritisk gumba za nadaljevanje programa
  Serial.println("/////////////////////////");
  Serial.println("Robota primite v roke in ga premikajte v vseh 3eh smereh, dokler ne izpiše Test končan.");
  Serial.println("Če želite prekiniti test, pritisnite gumb.");
  delay(2000); //zakasnitev, da debouncing

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
}
