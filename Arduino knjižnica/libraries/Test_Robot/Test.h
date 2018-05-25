/*
  Test.h - Library for testing the robot.
  Created by Miha Klokocovnik, March 3, 2018.
  Released into the public domain.
*/
#ifndef Test_h
#define Test_h

#include "Arduino.h"
#include "Robot.h"
//#include "Motor.h"
//#include "MPU9250.h"
//#include "Line.h"
//#include "Distance.h"

#define duration1000ms 1000 //casovnik dolzine 1000ms
#define LEFT 0 //levi motor se klice z besedo LEFT
#define RIGHT 1 //desni motor se klice z besedo RIGHT

class Test
{
  public:
    //klic funkcije v Arduino IDE okolju je npr.: test.battery();
    Test();
    void motor(uint8_t motor);
    void distance();
    void line();
    void buzzer();
    void battery();

    //NE DELUJE NA 2. VERZIJI ROBOTOV
    /*
    void temp();
    void mag();
    void accel();
    void gyro();
    */

    Robot robot;

  private:

};
extern Test test;
#endif
