#ifndef MOTOR_H 
#define MOTOR_H

#include "Filter.h"

class Motor {
private:
    Filter filter;

public:
    const int MIN_THROTTLE = 0;
    const int MAX_THROTTLE = 255;
    const int throttlePin;

    Motor(int throttlePin) : filter(0.8), throttlePin(throttlePin) {}

    void setThrottle(int);
    void test();

    void init() {
        pinMode(throttlePin, OUTPUT);
    }
};

void Motor::setThrottle(int pwm) {
    pwm = filter.read(pwm);
    // map pwm to motor
    analogWrite(throttlePin, pwm);
}

void Motor::test() {
  Serial.println("Testing Throttle Motor...");
  int gradient = 1;
  int throttle_pwm = MIN_THROTTLE + gradient;
  while(throttle_pwm != MIN_THROTTLE) { 
    if(throttle_pwm == MAX_THROTTLE) gradient = -1;
    setThrottle(throttle_pwm);
    Serial.println(throttle_pwm);
    delay(25);
    throttle_pwm += gradient;
  }
  setThrottle(throttle_pwm);
}

#endif
