#include "Distance.h"

/*
 * Constructor.
 */
Distance::Distance(uint8_t S0, uint8_t S1, uint8_t S2, uint8_t LED, uint8_t SEN)
{
    this->S0 = S0;
    this->S1 = S1;
    this->S2 = S2;
    this->LED = LED;
    this->SEN = SEN;
}

/*
 * Reads raw distance sensor data.
 */
void Distance::readRaw()
{
    ;
    int16_t onValues[7] = {1023, 1023, 1023, 1023, 1023, 1023, 1023};
    digitalWrite(LED, LOW);
    delayMicroseconds(500);
    for (uint8_t i = 1; i < 8; i++)
    {
        digitalWrite(S0, bitRead(i, 0));
        digitalWrite(S1, bitRead(i, 1));
        digitalWrite(S2, bitRead(i, 2));
        delayMicroseconds(5);
        onValues[i - 1] = analogRead(SEN);
    }
    digitalWrite(LED, HIGH);
    delayMicroseconds(500);
    for (uint8_t i = 1; i < 8; i++)
    {
        digitalWrite(S0, bitRead(i, 0));
        digitalWrite(S1, bitRead(i, 1));
        digitalWrite(S2, bitRead(i, 2));
        delayMicroseconds(5);
        sensorsRaw[i - 1] = onValues[i - 1] - analogRead(SEN);
    }
}
