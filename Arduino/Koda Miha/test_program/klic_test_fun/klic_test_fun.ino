//#include <Robot.h>
#include <Test.h>

void setup() {
  while (!Serial);
  Serial.println("Zapuscas setup, pritisni gumb na robotu za nadaljevanje!");
}

void loop() {
  if(robot.buttonPressed()){
    delay(500);
    //test.battery();
    //Serial.println(robot.motor[LEFT].getSpeed());
    //test.motor('L');
    //test.rightMotor();
    //test.buzzer();
    //test.temp();
    //test.mag();
    //test.accel();
    //test.gyro();
    //test.distance();
    test.line();
  }

  delay(100);
}
