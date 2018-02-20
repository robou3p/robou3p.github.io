#ifndef LINE_H
#define LINE_H

#define BLACK 0
#define WHITE 1

#include "Arduino.h"
#include <math.h>

class Line
{
  public:
    Line(uint8_t S0, uint8_t S1, uint8_t S2, uint8_t LED, uint8_t SEN);
    void calibrate(int16_t duration);
    float getPosition();
    float getAngle();
    float setThresholdLine(int16_t threshold);
    float setThresholdInclude(int16_t threshold);
    float setColor(uint8_t color);
    uint8_t getOnLine();
    uint8_t getOnLine(uint8_t sensor);
    void setLength(float length);

  private:
    void readRaw();
    void readCalibrated();
    uint8_t S0 = 0;
    uint8_t S1 = 0;
    uint8_t S2 = 0;
    uint8_t LED = 0;
    uint8_t SEN = 0;
    int16_t sensorsRaw[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int16_t sensorsMin[8] = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};
    int16_t sensorsMax[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int16_t sensorsCalibrated[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int16_t onLine = 0;
    int16_t linePrevious = 0;
    int16_t thresholdLine = 500;
    int16_t thresholdInclude = 500;
    uint8_t color = BLACK;
    float length = 0.082;
};

#endif
