#include "Motor.h"

/*
 * Constructor - define which pins the motor uses
 */
Motor::Motor(uint8_t IN1, uint8_t IN2, uint8_t I, uint8_t ENCD)
{
    this->IN1 = IN1;
    this->IN2 = IN2;
    this->I = I;
    this->ENCD = ENCD;
}

/*
 * Returns the voltage on the motor.
 */
float Motor::getVoltage()
{
    return voltage;
}

void Motor::setVoltage(float u)
{
    voltage = u;
    int16_t pwm = (int16_t)(voltage * 255.0 / 5.0);
    if (pwm > 255)
        pwm = 255;
    else if (pwm < -255)
        pwm = -255;
    if (IN1 == 11) // ToDo: left wheel hack, remove from future versions
    {
        if (pwm >= 0)
        {
            digitalWrite(IN2, 0);
            analogWrite(IN1, pwm);
        }
        else
        {
            digitalWrite(IN2, 1);
            analogWrite(IN1, 255 + pwm);
        }
    }
    else
    {
        if (pwm >= 0)
        {
            digitalWrite(IN1, 1);
            analogWrite(IN2, 255 - pwm);
        }
        else
        {
            digitalWrite(IN1, 0);
            analogWrite(IN2, -pwm);
        }
    }
}

/*
 * Gets the omega of the wheel in rad/s
 */
float Motor::getSpeed()
{
    return speed;
}

/*
 * PID speed control, input in rad/s
 */
void Motor::setSpeed(float w)
{
    uint32_t now = micros();
    uint32_t motorDt = now - motorLast;
    motorLast = now;
    float error = w - speed;
    errorIntegral = errorIntegral + error * motorDt / 1000000.0;
    if (errorIntegral > errorIntegralLimit)
        errorIntegral = errorIntegralLimit;
    else if (errorIntegral < -errorIntegralLimit)
        errorIntegral = -errorIntegralLimit;
    int16_t errorDifferential = (error - errorPrevious) * 1000000.0 / motorDt;
    setVoltage(kp * error + ki * errorIntegral + kd * errorDifferential);
    errorPrevious = error;
}

/*
 * Set gains for speed PID control;
 */
void Motor::setGains(float kp, float ki, float kd)
{
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;
}

/*
 * Sets the PID integral limit.
 */
void Motor::setILimit(float limit)
{
    errorIntegralLimit = limit;
}

/*
 * Return current in Amps
 */
float Motor::getCurrent()
{
    return analogRead(I) * 5.0 / 3276.8;
}

/*
 * Return distance in m.
 */
float Motor::getDistance()
{
    return encoderTicks * diameter * 0.008767561547;
}

/*
 * Reset distance to 0.
 */
void Motor::resetDistance()
{
    encoderTicks = 0;
}

/*
 * Get the wheel diameter in m.
 */
float Motor::getDiameter()
{
    return diameter;
}

/*
 * Set the wheel diameter in m.
 */
void Motor::setDiameter(float diameter)
{
    this->diameter = diameter;
}

/*
 * Timer 1 and 3input capture interrupt service routine
 */
void Motor::CAPT_ISR(uint8_t motor)
{
    uint32_t now = micros();
    encoderDt = now - encoderLast[3];
    encoderLast[3] = encoderLast[2];
    encoderLast[2] = encoderLast[1];
    encoderLast[1] = encoderLast[0];
    encoderLast[0] = now;
    if (motor == LEFT)
        // detect both rising and falling
        TCCR1B &= ~(1 << ICES1);
    else if (motor == RIGHT)
        // detect both rising and falling
        TCCR3B &= ~(1 << ICES3);
    encoderDirection = digitalRead(this->ENCD) ? -1 : 1;
    encoderTicks += encoderDirection;
    encoderOverflows = 0;
    speed = encoderDirection * /*84168.59085304*/ 140280.98475507 / (float)encoderDt;
}

/*
 * Timer 1 and 3 overflow interrupt service routine
 */
void Motor::OVF_ISR()
{
    if (++encoderOverflows > 64)
    {
        speed = 0.0;
    }
}
