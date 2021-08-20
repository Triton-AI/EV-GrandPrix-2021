#ifndef STEERING_WHEEL_H
#define STEERING_WHEEL_H

class SteeringWheel {
private:
public:
    const int MIN_ANGLE = -60;
    const int MAX_ANGLE = 60;
    const int minSwitch;
    const int maxSwitch;

    SteeringWheel(int switchA, int switchB) : minSwitch(switchA), maxSwitch(switchB) {}

    void setSteer(int theta);
    void test();
    
    void init() {
        pinMode(minSwitch, INPUT_PULLUP);
        pinMode(maxSwitch, INPUT_PULLUP);
    }
};

void SteeringWheel::setSteer(int theta) {}

void SteeringWheel::test() {
  Serial.println("Testing Steering Falcon 500 Motor...");
  int gradient = 1;
  int steering_angle = MIN_ANGLE + gradient;
  while(steering_angle != MIN_ANGLE) {
    if(steering_angle >= MAX_ANGLE) gradient = -1 * gradient;
    setSteer(steering_angle);
    Serial.println(steering_angle);
    delay(25);
    steering_angle += gradient;
  }
}

#endif
