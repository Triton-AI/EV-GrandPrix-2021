#ifndef STEERING_WHEEL_H
#define STEERING_WHEEL_H

class SteeringWheel {
private:
public:
    const int MIN_STEER = -60;
    const int MAX_STEER = 60;
    const int minSwitch;
    const int maxSwitch;

    SteeringWheel(int switchA, int switchB) : minSwitch(switchA), maxSwitch(switchB) {}

    void setSteer(int theta);
    void init() {
        pinMode(minSwitch, INPUT_PULLUP);
        pinMode(maxSwitch, INPUT_PULLUP);
    }
};

void SteeringWheel::setSteer(int theta) {
    if(!digitalRead(minSwitch)) {
        Serial.print("Reached Minimum Steering Angle: ");
        Serial.println(theta);
    }
    else if(!digitalRead(maxSwitch)) {
        Serial.print("Reached Maximum Steering Angle: ");
        Serial.println(theta);
    }
    return;
}

#endif