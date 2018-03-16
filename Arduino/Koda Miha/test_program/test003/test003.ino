#include <Robot.h>
/*
  int16_t duration1000ms = 1000; //dolzina casovnika v [ms]
  uint32_t tStart = millis();
  do{
    
  } while (millis() < tStart + duration1000ms);
  
  uint32_t timer1 = millis();
  if (millis() > timer1 + duration1000ms){
    //tu das kodo ki naj se izvede vsako ekundo
    //npr. izpis na serijski monitor
  }
*/



void setup() {
  while (!Serial); //nic se ne zgodi dokler ne odpres serial monitorja
  robot.imu.begin(); //da se zacne kom. s senzorji
  Serial.println("Za začetek testa pritisni gumb na desni strani robota (tisti, ki NI zraven USB kabla)!");
  Serial.println();
  while (!robot.buttonPressed());
  Serial.println("/////////////////////////");
  delay(2000); //zakasnitev, da debouncing
  
  //TEST BRENCAC
  /*
  Serial.println("Robot bo piskal 5 sekund. Če želiš prekiniti test pritisni gumb.");
  int16_t frequency = 500;
  int16_t timeBuzzerON = 100;
  int16_t error = 0;
  uint32_t tStart1 = millis();
  do {
    robot.beep(frequency, timeBuzzerON);
    uint32_t tStart2 = millis();
    do{
      if (robot.buttonPressed()){
        error = 1;
        Serial.println("Prekinili ste test!");
        break;
      }
    } while (millis() < tStart2 + duration1000ms/10);
    if (error) break;
    if (robot.buttonPressed()){
        Serial.println("Prekinili ste test!");
        break;
      }
  } while (millis() < tStart2 + duration1000ms*5)
  
  */
  //TEST LEVI MOTOR
  /*
  float U = 3.0;
  while (!robot.buttonPressed()){
    robot.motor[LEFT].setVoltage(U);
    delay(10);
  }
  robot.motor[LEFT].setVoltage(0);
  */
  //TEST DESNI MOTOR
  /*
  Serial.println("Robota dvigni v zrak in pritisni gumb, za začetek testa desnega motorja.");
  while (!robot.buttonPressed());
  delay(200);
  float U = 2.0;
  int rez = 0;
  Serial.println("Če želiš prekiniti testiranje motorja med izvajanjem, ponovno pritisni gumb.");
  while (!rez){
    if (robot.buttonPressed()){
      break;
    }
    robot.motor[RIGHT].setVoltage(U);
    delay(2000);
    if (robot.motor[RIGHT].getSpeed() > 5.0){
      Serial.println("Desni motor se pravilno vrti naprej.");
      robot.motor[RIGHT].setVoltage(-U);
      delay(2000);
      if (robot.motor[RIGHT].getSpeed() < -5.0){
        Serial.println("Desni motor se pravilno vrti nazaj.");
        rez = 1;
      }
    }
  }
  robot.motor[RIGHT].setVoltage(0);
  */
  //TEST BATERIJA
  /*
  Serial.print("Baterija ima ");
  Serial.print(robot.battery());
  Serial.println("V napetosti.");
  */
  //TEST TEMPERATURA
  /*
  robot.imu.readSensor();
  float T = robot.imu.getTemperature_C();
  Serial.println(T);
  */
  //TEST MAGNETNO POLJE
  
  Serial.println("");
  Serial.println("Meritev magnetnega polja se izpisuje na 1s dokler ne pritisneš gumba.");
  while (!robot.buttonPressed()){
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
    delay(1000); //namesto tega uvedi timerje!!! drugace deluje pocasi
  }
  
  //TEST POSPESKOMETER
  /*
  robot.imu.readSensor();
  Serial.print("Pospešek v m/s2 je: ");
  Serial.print("X=");
  Serial.print(robot.imu.getAccelX_mss());
  Serial.print("  Y=");
  Serial.print(robot.imu.getAccelY_mss());
  Serial.print("  Z=");
  Serial.println(robot.imu.getAccelZ_mss());
  delay(200); //uporabi timer + dodaj while zanko
  */
  //TEST ZIROSKOP
  /*
  int tmpX, tmpY, tmpZ = 0;
  float prag = 1.0; //nastavi prag obcutljivosti [rad/s], ki ga mora senzor preseci, da opravi test
  Serial.println("Začel se je test žiroskopa, prekinete ga s pritiskom gumba.");
  Serial.println("Robota primite v roke in ga premikajte v vseh 3eh smereh, dokler ne izpiše Test končan.");
  delay(100);
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
    
    if (abs(gyroX) > prag && tmpX == 0){
      tmpX = 1; //preveri ce je zaznano gibanje mocnejse od paga
      Serial.println("X - OK"); //ZAKAJ TEGA NE IZPISUJE?????????????????????????????????????????
    }
    if (abs(gyroY) > prag) tmpY = 1;
    if (abs(gyroZ) > prag) tmpZ = 1;
    if (tmpX && tmpY && tmpZ){ //ce je zaznano gibanje v vseh 3eh smereh opozori uporabnika in koncaj test
      Serial.println("Žiroskop deluje!  Test zaključen.");
      break;
    }
    delay(100);
  }
  */

  Serial.println("ZAPUSCAS SETUP!!!!!!");
}

void loop() {
  /*
  
  */
  
  //Ce pritisnes gumb se ustavi celotno izvajanje  
  if (robot.buttonPressed()){
    robot.motor[RIGHT].setVoltage(0);
    robot.motor[LEFT].setVoltage(0);
    delay(2000);
    while(!robot.buttonPressed());
  }
  delay(100);
}
