#ifndef DISTANCE_H
#define DISTANCE_H

#include "Arduino.h"

class Distance
{
  public:
    Distance(uint8_t S0, uint8_t S1, uint8_t S2, uint8_t LED, uint8_t SEN);
    void readRaw();
    int16_t sensorsRaw[8] = {0, 0, 0, 0, 0, 0, 0, 0};

  private:
    uint8_t S0 = 0;
    uint8_t S1 = 0;
    uint8_t S2 = 0;
    uint8_t LED = 0;
    uint8_t SEN = 0;
};

#endif
