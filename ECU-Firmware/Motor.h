#ifndef MOTOR_H 
#define MOTOR_H

#include "Filter.h"

class Motor {
private:
    Filter filter;

public:
    const int MIN_THROTTLE = 160;
    const int MAX_THROTTLE = 885;
    const int throttlePin;

    Motor(int throttlePin) : filter(0.8), throttlePin(throttlePin) {}

    void setThrottle(int);

    void init() {
        pinMode(throttlePin, OUTPUT);
    }
};

void Motor::setThrottle(int pwm) {
    pwm = filter.read(pwm);
    // map pwm to motor
    analogWrite(throttlePin, pwm);
}

#endif
