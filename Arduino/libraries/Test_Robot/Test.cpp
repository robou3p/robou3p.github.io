/*
  Test.cpp - Library for testing the robot.
  Created by Miha Klokocovnik, March 3, 2018.
  Released into the public domain.
*/

#include "Test.h"
#include "Arduino.h"
//#include "Robot.h"
//#include "Motor.h"
// #include "MPU9250.h"
// #include "Line.h"
// #include "Distance.h"

Test::Test(){
  //robot.imu.begin();
}

Test test = Test();

/*
  Izpise ali motor deluje pravilno ali ne.
*/
void Test::motor(uint8_t motor){
  //preverjanje pravilnosti vhodnega parametra
  if (motor != LEFT && motor != RIGHT){
    Serial.println("Napačen vhodni prarameter funkcije. Vpiši LEFT ali RIGHT.");
    return;
  }
  while (!Serial); //nic se ne zgodi dokler ne odpres serial monitorja
  Serial.println("Robota dvignite v zrak in pritisnite funkcijski gumb!");
  while (!robot.buttonPressed()); //caka uporabnika
  Serial.println("Če želite prekiniti test med izvajanjem, ponovno pritisnite gumb.");
  Serial.println();
  delay(2000); //zakasnitev, da debouncing
  float U = 4.0; //napetost motorja
  int rez = 0; //ce je 1, potem je ze uspesno prestal test
  while (!rez){
    if (robot.buttonPressed()){ //preveri ali uporabnik zeli prekiniti izvajanje testa
      break;
    }
    robot.motor[motor].setSpeed(U); //nastavi napetost motorja
    delay(1000);

    if (robot.motor[motor].getVoltage() == 0.0){
      Serial.println("Napaka! Funkcija robot.motor[].getVoltage() vrača 0.00");
      delay(5);
      break;
    }
    if (robot.motor[motor].getVoltage() > 3.0){
      Serial.println("Motor se pravilno vrti naprej.");
      robot.motor[motor].setVoltage(-U);
      delay(1000);
      if (robot.motor[motor].getVoltage() < -3.0){
        Serial.println("Motor se pravilno vrti nazaj.");
        rez = 1;
      }
    }
  }
  robot.motor[motor].setVoltage(0); //nastavi hitrost motorja na 0
}

/*
  Izpise MIN in MAX prebrane vrednosti posameznih senzorjev blizine.
*/
void Test::distance(){
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
    Serial.println("Napaka! Meritve vseh senzorjev so enake 0.");
    return;
  }
  delay(5);
  Serial.println("Ko so ovire odstranjene, pritisni funkcijski gumb na robotu za nadaljevanje.");
  Serial.println();
  delay(500);
  //sedaj preverjas ce so se vrednosti spremenile in jih prepises ce zadostijo pogoju
  //to delas dokler uporabnik ne pritisne gumba na robotu
  while (!robot.buttonPressed()){ //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
  Serial.println("Pritisni funkcijski gumb za nadaljevanje.");
  delay(1000);
  Serial.println();
  while(!robot.buttonPressed()); //caka na uporabnika
  delay(1000); //debouncing
  Serial.println("START kalibracije");
  robot.beep(200,500); //pisk za zacetek kalibracije
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
}

/*
  Uporabniku sporoci, ali senzorji za crto delujejo pravilno.
*/
void Test::line(){
  while(!Serial);
  delay(1000);
  //kalibracija senzorjev
  Serial.println("Kalibracija traja 5s. Začne in konča se s piskom.");
  Serial.println("Pritisni funkcijski gumb za začetek.");
  Serial.println();
  while(!robot.buttonPressed()); //caka na uporabnika
  delay(1000); //debouncing
  Serial.println("START kalibracije");
  robot.beep(200,500); //pisk za zacetek kalibracije
  robot.line.calibrate(5000); //kalibracija 5000 ms
  robot.beep(200,500); //pisk za konec kalibracije
  Serial.println("KONEC kalibracije");
  delay(10);
  Serial.println();
  Serial.println("Pritisni gumb za preverjanje uspešnosti kalibracije.");
  while(!robot.buttonPressed()); //caka na uporabnika
  delay(1000); //debouncing
  Serial.println();
  Serial.println("Črto daj pod SREDINO robota.");
  while(!robot.buttonPressed()){ //ce uporabnik pritisne gumb, prekine ta del testa
    if ((robot.line.getPosition() * 100) < 0.3 && (robot.line.getPosition() * 100) > -0.3){ //ce je crta manj kot 1 cm od sredine robota
      Serial.println("--SREDINA--");
      robot.beep(500,300);
      break;
    }
  }
  delay(2000);
  Serial.println();
  Serial.println("Črto daj pod LEVI bok robota.");
  while(!robot.buttonPressed()){ //ce uporabnik pritisne gumb, prekine ta del testa
    if ((robot.line.getPosition() * 100) < -2.5){ //ce je crta dlje kot 2,5 cm od sredine robota proti levi
      Serial.println("--LEVA--");
      robot.beep(500,300);
      break;
    }
  }
  delay(2000);
  Serial.println();
  Serial.println("Črto daj pod DESNI bok robota.");
  while(!robot.buttonPressed()){ //ce uporabnik pritisne gumb, prekine ta del testa
    if ((robot.line.getPosition() * 100) > 2.5){ //ce je crta dlje kot 2,5 cm od sredine robota proti desni
      Serial.println("--DESNA--");
      robot.beep(500,300);
      break;
    }
  }
  delay(10);
  Serial.println();
}

/*
  Vklopi brencac za 5 sekund.
*/
void Test::buzzer(){
  while (!Serial); //nic se ne zgodi dokler ne odpres serial monitorja
  Serial.println("Za začetek testa BRENČAČA pritisnite funkcijski gumb!");
  Serial.println();
  while (!robot.buttonPressed()); //caka na pritisk gumba za nadaljevanje programa
  Serial.println("Če želite prekiniti test, ponovno pritisnite gumb.");
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
  Izpise napetost baterije v stavku.
*/
void Test::battery(){
  while (!Serial); //nic se ne zgodi dokler ne odpres serial monitorja
  if (robot.battery() == 0.0){ //ce je napetost baterije enaka 0.0 opozori uporabnika in konca test
    Serial.println("Napaka! Baterija ni vstavljena.");
    Serial.println();
    return;
  }
  Serial.print("Baterija ima ");
  Serial.print(robot.battery()); //prebere napetost baterije v [V]
  Serial.println("V napetosti.");
  Serial.println();
}

/*
  Enkrat izpiše temperaturo v enoti [C].
*/
/*
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
*/
/*
  Enkrat izpiše magnetno polje v X,Y,Z smeri v enoti [uT].
*/
/*
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
*/
/*
  Preveri ali je pospešek v X,Y,Z smeri večji od praga. Enote so [m/s2].
*/
/*
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
    //ce je bil v doloceni smeri ze presezen prag, se ta smer neha izpisovati
    if (!tmpX){
      Serial.print("X=");
      Serial.print(accelX);
    }
    if (!tmpY){
      Serial.print("  Y=");
      Serial.print(accelY);
    }
    if (!tmpZ){
      Serial.print("  Z=");
      Serial.print(accelZ);
    }

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
*/
/*
  Preveri ali je premik v X,Y,Z smeri večji od praga. Enote so [rad/s].
*/
/*
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
    //ce je bil v doloceni smeri ze presezen prag, se ta smer neha izpisovati
    if (!tmpX){
      Serial.print("X=");
      Serial.print(gyroX);
    }
    if (!tmpY){
      Serial.print("  Y=");
      Serial.print(gyroY);
    }
    if (!tmpZ){
      Serial.print("  Z=");
      Serial.print(gyroZ);
    }

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
*/
