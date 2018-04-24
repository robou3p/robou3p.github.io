#include <Robot.h>
#include <Test.h>

void setup() {
  while (!Serial);
  Serial.println("Zapuscas setup!");
}

void loop() {
  if(robot.buttonPressed()){
    test.battery();
    test.motor('R');
    test.buzzer();
    test.distance();
    test.line();
    //To spodaj ne deluje z 2. verzijo robota
    //test.temp();
    //test.mag();
    //test.accel();
    //test.gyro();
  }

  delay(100);
}
