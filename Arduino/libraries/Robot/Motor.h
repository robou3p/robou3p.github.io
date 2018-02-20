#ifndef MOTOR_H
#define MOTOR_H

#include "Arduino.h"

#define TIMER1 1
#define TIMER3 3

#define LEFT 0
#define RIGHT 1

class Motor
{
public:
  Motor(uint8_t IN1, uint8_t IN2, uint8_t I, uint8_t ENCD);
  float getVoltage();
  void setVoltage(float u);
  float getSpeed();
  void setSpeed(float w);
  void setGains(float kp, float ki, float kd);
  void setILimit(float limit);
  float getCurrent();
  float getDistance();
  void resetDistance();
  float getDiameter();
  void setDiameter(float diameter);

  void CAPT_ISR(uint8_t motor);
  void OVF_ISR();

  volatile int32_t encoderTicks = 0;

private:
  float diameter = 0.032;
  float voltage = 0.0;
  float speed = 0.0;
  float current = 0.0;
  float distance = 0.0;
  float kp = 0.15;
  float ki = 1.0;
  float kd = 0.0;
  uint8_t IN1 = 0;
  uint8_t IN2 = 0;
  uint8_t I = 0;
  uint8_t ENCD = 0;
  volatile uint32_t encoderDt = 0;
  volatile int8_t encoderDirection = 1;
  
  volatile uint32_t encoderLast[4] = {0, 0, 0, 0};
  volatile uint32_t encoderOverflows = 0;
  volatile uint32_t motorLast = 0;
  float errorIntegral = 0.0;
  float errorIntegralLimit = 3.0;
  float errorPrevious = 0.0;
};

#endif
