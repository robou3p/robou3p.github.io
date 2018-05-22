#include <Robot.h>

void setup() {
  while (!Serial);
}

void loop() {
  //()()()()()()()()()()()()()()()()()()()()()()()()()()()()()
  
  //()()()()()()()()()()()()()()()()()()()()()()()()()()()()()
  return;


}

//DEFINICIJE FUNKCIJ
float temperatura(){
  robot.imu.readSensor(); //branje pod. iz senzorskega cipa
  float T = robot.imu.getTemperature_C()); //zapis vrednosti v spremenljivko
  return T;
}
float magnetnoPolje(){
  robot.imu.readSensor(); //branje pod. iz senzorskega cipa
  float magX = robot.imu.getMagX_uT(); //zapis vrednosti v spremenljivke
  float magY = robot.imu.getMagY_uT();
  float magZ = robot.imu.getMagZ_uT();
  float absMag = float() sqrt(sq(magX)+sq(magY)+sq(magZ)); //izracun absolutne vrednosti polja
  return absMag;
}
float relativenPospesek(){
  float accelX, accelY, accelZ, tmpX, tmpY, tmpZ;
  int i = 0;
  //for zanka vnese zakasnitev priblizno: i x 1ms
  //aritmeticno povpreci i meritev za odstranitev suma
  for (i; i < 100; i++){
    robot.imu.readSensor();
    accelX = robot.imu.getAccelX_mss();
    accelY = robot.imu.getAccelY_mss();
    accelZ = robot.imu.getAccelZ_mss();
    tmpX = tmpX + accelX;
    tmpY = tmpY + accelY;
    tmpZ = tmpZ + accelZ;
    delay 1;
  }
  tmpX = tmpX / i; //deli vsoto vseh meritev s stevilom meritev
  tmpY = tmpY / i;
  tmpZ = tmpZ / i;
  float relAccel = float() sqrt(sq(tmpX)+sq(tmpY)+sq(tmpZ)) - 10; //odsteje 10[m/s^2] za relatien pospesek 
  return relAccel;
}
float pospesekX(){
  robot.imu.readSensor();
  float accelX = robot.imu.getAccelX_mss();
  return accelX;
}
float pospesekY(){
  robot.imu.readSensor();
  float accelY = robot.imu.getAccelY_mss();
  return accelY;
}
float pospesekZ(){
  robot.imu.readSensor();
  float accelZ = robot.imu.getAccelZ_mss();
  return accelZ;
}



