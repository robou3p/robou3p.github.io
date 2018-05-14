#include "Robot.h"

/*
 * Constructor
 */
Robot::Robot()
{
    pinMode(ENCAI, INPUT_PULLUP);
    pinMode(AIN1, OUTPUT);
    digitalWrite(AIN1, LOW);
    pinMode(BIN1, OUTPUT);
    digitalWrite(BIN1, LOW);
    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, LOW);
    pinMode(BUTTON, INPUT);
    pinMode(ENCBD, INPUT);
    pinMode(AIN2, OUTPUT);
    digitalWrite(AIN2, LOW);
    pinMode(ENCAD, INPUT);
    pinMode(ENCBI, INPUT_PULLUP);
    pinMode(BIN2, OUTPUT);
    digitalWrite(BIN2, LOW);
    pinMode(MUXL2, OUTPUT);
    digitalWrite(MUXL2, LOW);
    pinMode(MUXL0, OUTPUT);
    digitalWrite(MUXL0, LOW);
    pinMode(MUXL1, OUTPUT);
    digitalWrite(MUXL1, LOW);
    pinMode(MMLED, OUTPUT);
    digitalWrite(MMLED, HIGH);
    pinMode(LFLED, OUTPUT);
    digitalWrite(LFLED, LOW);
    pinMode(ENCAI, INPUT_PULLUP);
    pinMode(ENCBI, INPUT_PULLUP);

    // make sure that the robot is stopped
    motor[LEFT].setVoltage(0);
    motor[RIGHT].setVoltage(0);
    setupEncoders();
    Serial.begin(115200);
    Serial1.begin(115200);
}

/*
 * Drive with v (m/s) and w(rad/s).
 */
void Robot::drive(float v, float w)
{
    float leftd = motor[LEFT].getDiameter();
    float rightd = motor[RIGHT].getDiameter();
    motor[LEFT].setSpeed(2 * v / leftd - w * width / leftd);
    motor[RIGHT].setSpeed(2 * v / rightd + w * width / rightd);
}

/*
 * Drive forward for certain distance (m).
 */
void Robot::go(float distance)
{
    robot.motor[LEFT].resetDistance();
    for(int i = 0; i < 1000000; i++){
        robot.motor[LEFT].setVoltage(0.9*distance/abs(distance));
        robot.motor[RIGHT].setVoltage(0.9*distance/abs(distance));
        if(abs(robot.motor[LEFT].getDistance()) >= abs(distance)){
            robot.motor[LEFT].setVoltage(0);
            robot.motor[RIGHT].setVoltage(0);
            delay(250);
            break;
        }
    }
}

/*
 * Rotate for certain angle (°).
 */
void Robot::turn(float angle)
{
    robot.motor[LEFT].resetDistance();
    robot.motor[RIGHT].resetDistance();
    for(int i = 0; i < 1000000; i++){
        if(i < 4){
            motor[LEFT].setSpeed(-i*angle/abs(angle));
            motor[RIGHT].setSpeed(i*angle/abs(angle));
        }
        else{
            motor[LEFT].setSpeed(-4*angle/abs(angle));
            motor[RIGHT].setSpeed(4*angle/abs(angle));
        }
        if((abs(robot.motor[LEFT].getDistance()) >= abs(0.000705*angle)) || (abs(robot.motor[RIGHT].getDistance()) >= abs(0.000705*angle))){
            robot.motor[LEFT].setVoltage(0);
            robot.motor[RIGHT].setVoltage(0);
            delay(250);
            break;
        }
    }
}

/*
 * Beeps with the specified frequency and duration.
 */
void Robot::beep(int16_t frequency, int16_t duration)
{
    int32_t tStart = micros();
    int16_t t = (int16_t)(500000L / (int32_t)frequency);
    while (micros() < tStart + (int32_t)duration * 1000)
    {
        digitalWrite(BUZZER, HIGH);
        delayMicroseconds(t);
        digitalWrite(BUZZER, LOW);
        delayMicroseconds(t);
    }
}

/*
 * Returns the state of the button.
 */
uint8_t Robot::buttonPressed()
{
    return !digitalRead(BUTTON);
}

/*
 * Returns the battery voltage.
 */
float Robot::battery()
{
    digitalWrite(MUXL0, LOW);
    digitalWrite(MUXL1, LOW);
    digitalWrite(MUXL2, LOW);
    return analogRead(MMSEN) * 5.0 / 1024.0;
}

/*
 * Sets the distance between the wheels.
 */
void Robot::setWidth(float width)
{
    this->width = width;
}

/*
 * Sets up the TIMER1 and TIMER3 interrupts.
 */
void Robot::setupEncoders()
{
    // Input Capture setup
    // ICNC1: =0 Disable Input Capture Noise Canceler to prevent delay in reading
    // ICES1: =1 for trigger on rising edge
    // CS11: =1 set prescaler to 1/8 system clock (F_CPU)
    TCCR1A = 0;
    TCCR1B = (0 << ICNC1) | (1 << ICES1) | (1 << CS11);
    TCCR1C = 0;
    // Interrupt setup
    // ICIE1: Input capture, TOIE1: Overflow
    TIFR1 = (1 << ICF1);                  // clear pending
    TIMSK1 = (1 << ICIE1) | (1 << TOIE1); // and enable
    // Timer 3
    TCCR3A = 0;
    TCCR3B = (0 << ICNC3) | (1 << ICES3) | (1 << CS31);
    TCCR3C = 0;
    TIFR3 = (1 << ICF3);                  // clear pending
    TIMSK3 = (1 << ICIE3) | (1 << TOIE3); // and enable
}

Robot robot = Robot();

ISR(TIMER1_CAPT_vect)
{
    robot.motor[LEFT].CAPT_ISR(LEFT);
}

ISR(TIMER3_CAPT_vect)
{
    robot.motor[RIGHT].CAPT_ISR(RIGHT);
}

ISR(TIMER1_OVF_vect)
{
    robot.motor[LEFT].OVF_ISR();
}

ISR(TIMER3_OVF_vect)
{
    robot.motor[RIGHT].OVF_ISR();
}
