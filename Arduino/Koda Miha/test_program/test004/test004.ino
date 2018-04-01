#include <Robot.h>
#include <Test.h>

void setup() {
  while (!Serial);
  Serial.println("Zapuscas setup, pritisni gumb na robotu!");
}

void loop() {
  if(robot.buttonPressed()){
    delay(500);
    //test.battery();
    //test.leftMotor();
    //test.rightMotor();
    //test.buzzer();
    //test.temp();
    //test.mag();
    //test.accel();
    //test.gyro();
    //test.distance();
    //test.line();
  }

  delay(100);
}
