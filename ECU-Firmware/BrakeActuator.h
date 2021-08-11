#ifndef BRAKE_ACTUATOR_H
#define BRAKE_ACTUATOR_H

class BrakeActuator {
private:
public:
    const int MAX_POSITION = 0;
    const int MIN_POSITION = 255;
    
    BrakeActuator() {}

    void setPosition(int position);

    void init() {
      return;
    }
};

void BrakeActuator::setPosition(int position) {
    return;
}

#endif
