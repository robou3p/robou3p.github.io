#include <Distance.h>
#include <Filters.h>
#include <Line.h>
#include <Motor.h>
#include <MPU9250.h>
#include <Robot.h>
#include <Test.h>
#include <Util.h>


void setup() {
  while (!Serial);
}

void loop() {
  //test.battery();
  //test.buzzer();
  test.temp(); //ne dela
  //test.rightMotor(); //ne dela

  delay(2000);
}
