/*
  Test.h - Library for testing the robot.
  Created by Miha Klokocovnik, March 3, 2018.
  Released into the public domain.
*/
#ifndef Test_h
#define Test_h

#include "Arduino.h"
#include "Robot.h"
#include "Motor.h"
#include "MPU9250.h"
#include "Line.h"
#include "Distance.h"
#include <avr/interrupt.h>
#include <math.h>
#include "Wire.h"

#define duration1000ms 1000 //casovnik dolzine 1000ms

class Test
{
  public:
    //klic funkcije v Arduino IDE okolju je npr.: test.battery();
    void battery();
    void leftMotor();
    void rightMotor();
    void buzzer();
    void temp();
    void mag();
    void accel();
    void gyro();
    void distance();
    void line();

    Robot robot;

  private:

};
extern Test test;
#endif
