#include <Robot.h>

void setup() {
  while (!Serial);
  Serial.println("Zapuscas setup, pritisni gumb na robotu!");
}

void loop() {
  if(robot.buttonPressed()){
    delay(500);
    new
  }

  delay(100);
}
