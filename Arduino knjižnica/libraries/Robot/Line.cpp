#include "Line.h"

/*
 * Constructor
 */
Line::Line(uint8_t S0, uint8_t S1, uint8_t S2, uint8_t LED, uint8_t SEN)
{
    this->S0 = S0;
    this->S1 = S1;
    this->S2 = S2;
    this->LED = LED;
    this->SEN = SEN;
}

/*
 * Line calibration, duration in ms.
 */
void Line::calibrate(int16_t duration)
{
    uint32_t tStart = millis();
    do
    {
        readRaw();
        for (uint8_t i = 0; i < 8; i++)
        {
            if (sensorsRaw[i] > sensorsMax[i])
                sensorsMax[i] = sensorsRaw[i];
            if ((sensorsRaw[i] < sensorsMin[i]))
                sensorsMin[i] = sensorsRaw[i];
        }
    } while (millis() < tStart + duration);
}

/*
 * Weighted average in m from centre.
 */
float Line::getPosition()
{
    readCalibrated();
    int32_t weightedSum = 0;
    int16_t sum = 0;
    onLine = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        int value = sensorsCalibrated[i];
        if (color == WHITE)
        {
            value = 1023 - value;
        }
        if (value > thresholdLine)
        {
            onLine += (1 << i);
        }
        if (value > thresholdInclude)
        {
            weightedSum += (int32_t)value * i * 1000;
            sum += value;
        }
    }
    if (onLine)
    {
        if (onLine & B00000001)
        {
            linePrevious = -3500;
        }
        else if (onLine & B10000000)
        {
            linePrevious = 3500;
        }
        else
        {
            linePrevious = (int16_t)(weightedSum / sum - 3500);
        }
    }
    else
    {
        if (linePrevious < 0)
            linePrevious = -3500;
        else
            linePrevious = 3500;
    }

    return linePrevious / 100000.0;
}

float Line::getAngle()
{
    return atan2(getPosition(), length);
}

float Line::setThresholdLine(int16_t threshold)
{
    thresholdLine = threshold;
}
float Line::setThresholdInclude(int16_t threshold)
{
    thresholdInclude = threshold;
}
float Line::setColor(uint8_t color)
{
    this->color = color;
}

/*
 * Returns a byte with sensor states (b11000000 means that the two left-most sensors are on line)
 */
uint8_t Line::getOnLine()
{
    return onLine;
}

/*
 * Returns whether the selected sensor is on line.
 */
uint8_t Line::getOnLine(uint8_t sensor)
{
    return bitRead(onLine, sensor);
}

/*
 * Set the length of the robot, i.e. the distance from centre of rotation to sensors.
 */
void Line::setLength(float length)
{
    this->length = length;
}

/*
 * Reads raw sensor values into sensorsRaw array.
 */
void Line::readRaw()
{
    int16_t sensorsOn[8] = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};
    digitalWrite(LED, HIGH);
    delayMicroseconds(500);
    for (uint8_t i = 0; i < 8; i++)
    {
        digitalWrite(S0, bitRead(i, 0));
        digitalWrite(S1, bitRead(i, 1));
        digitalWrite(S2, bitRead(i, 2));
        delayMicroseconds(2);
        sensorsOn[i] = analogRead(SEN);
    }
    digitalWrite(LED, LOW);
    delayMicroseconds(500);
    for (uint8_t i = 0; i < 8; i++)
    {
        digitalWrite(S0, bitRead(i, 0));
        digitalWrite(S1, bitRead(i, 1));
        digitalWrite(S2, bitRead(i, 2));
        delayMicroseconds(2);
        sensorsRaw[i] = sensorsOn[i] + 1023 - analogRead(SEN);
    }
}

/*
 * Reads calibrated values (between 0 and 1000).
 */
void Line::readCalibrated()
{
    readRaw();
    for (uint8_t i = 0; i < 8; i++)
    {
        sensorsCalibrated[i] = (int16_t)((((int32_t)(sensorsRaw[i] - sensorsMin[i])) * 1000) /
                                         (sensorsMax[i] - sensorsMin[i]));
        if (sensorsCalibrated[i] < 0)
            sensorsCalibrated[i] = 0;
        else if (sensorsCalibrated[i] > 1000)
            sensorsCalibrated[i] = 1000;
    }
}
