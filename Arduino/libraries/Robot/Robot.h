#ifndef ROBOT_H
#define ROBOT_H

#include "Arduino.h"
#include "Motor.h"
#include "Line.h"
#include "Distance.h"
#include "MPU9250.h"
#include "Filters.h"
#include "Util.h"
#include <avr/interrupt.h>
#include <math.h>
#include "Wire.h"

/*
 * pin definitions (see schematics)
 */
#define ENCAI 4  // PD4
#define ENCAD 5  // PC6
#define BIN2 6   // PD7
#define BUZZER 7 // PE6
#define BUTTON 8 // PB4
#define ENCBD 9  // PB5
#define AIN1 10  // PB7
#define AIN2 11  // PB6
#define ENCBI 13 // PC7
#define BIN1 12  // PD6
#define MUXL2 14 // PB3
#define MUXL0 15 // PB1
#define MUXL1 16 // PB2
#define MMLED 18 // PF7
#define LFLED 22 // PF1
#define MMSEN A1 // PF7
#define MBI A2   // PF5
#define MAI A3   // PF4
#define LFSEN A5 // PF0

class Robot
{
public:
  Robot();
  void drive(float v, float w);
  void beep(int16_t frequency, int16_t duration);
  uint8_t buttonPressed();
  float battery();
  void setWidth(float width);

  Motor motor[2] = {Motor(AIN1, AIN2, MAI, ENCAD), Motor(BIN1, BIN2, MBI, ENCBD)};
  Line line = Line(MUXL0, MUXL1, MUXL2, LFLED, LFSEN);
  Distance distance = Distance(MUXL0, MUXL1, MUXL2, MMLED, MMSEN);
  MPU9250 imu = MPU9250(Wire, 0x68);
  Filters filters;

private:
  void setupEncoders();

  float width = 0.084;
};

extern Robot robot;

#endif
