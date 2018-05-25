#include "Util.h"

float rads2rpm(float rads)
{
    return rads * 9.5492965855;
}
float rpm2rads(float rpm)
{
    return rpm * 0.1047197551;
}

float rad2deg(float rad)
{
    return rad * 57.2957795131;
}

float deg2rad(float deg)
{
    return deg * 0.01745329252;
}
