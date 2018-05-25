// Ardublockly generated sketch
#include <Robot.h>

void setup() {
}

void loop() {
  if (robot.buttonPressed()) {
    robot.beep(110, 1000);
    delay(1000);
    robot.beep(110, 1000);
  }

}