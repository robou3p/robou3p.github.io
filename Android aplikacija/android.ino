#include <Robot.h>

void setup() {

}

void loop() {

  if (Serial1.available()) {
    switch (Serial1.read()) {
      case ('L'): {
          while (!Serial1.available()) {}
          int dirLeft = Serial1.read();
          while (!Serial1.available()) {}
          int speedLeft = Serial1.read();
          if (speedLeft > 100)
            speedLeft = 100;
          if (dirLeft == 1) {
            Serial.println(speedLeft);
            robot.motor[LEFT].setVoltage(speedLeft / 50.0);
          } else {
            Serial.println(speedLeft);
            robot.motor[LEFT].setVoltage(-speedLeft / 50.0);
          }
          break;
        }
      case ('R'): {
          while (!Serial1.available()) {}
          int dirRight = Serial1.read();
          while (!Serial1.available()) {}
          int speedRight = Serial1.read();
          if (speedRight > 100) {
            speedRight = 100;
          }
          if (dirRight == 1) {
            Serial.println(speedRight);
            robot.motor[RIGHT].setVoltage(speedRight / 50.0);
          } else {
            Serial.println(speedRight);
            robot.motor[RIGHT].setVoltage(-speedRight / 50.0);
          }
          break;
        }
    }
  }
}
