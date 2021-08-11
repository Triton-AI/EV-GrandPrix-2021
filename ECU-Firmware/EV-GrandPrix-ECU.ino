/**
 * Ideas: 
 * Input from manual pedal
 *  - potentiometer
 *  - microswitch
 * The potentiometer will feed into the pedal emulator
 * The microswicth will feed into the ecu as an interupt
 * if the switch is on -> shut off the safety pulse
 *    let the pedal potentiometer control the motor
 * otherwise
 *    autonomous is enabled
 *    saftey pulse is on 1000Hz square wave
 *    read input from ros cube and control steering and thorttle
 */

#include "Motor.h"
#include "SteeringWheel.h"
#include "BrakeActuator.h"

const int safetyPin = 6; 
const int altraxThrottlePin = 5;
const int pedalPin = 4;
const int steerMinSwitch = 7;
const int steerMaxSwitch = 8;

Motor motor(altraxThrottlePin);
SteeringWheel wheel(steerMinSwitch, steerMaxSwitch);
BrakeActuator brake;

int throttle_pwm = 0;
int steering_theta = -60;
int actuator_position = 0;
int gradient = 0;

void setup() {
  Serial.begin(9600);

  /* Physical Pedal */
  pinMode(pedalPin, INPUT);
  /* Pedal Emulator */
  pinMode(safetyPin, OUTPUT);
  analogWrite(safetyPin, 127); // enable pedal emulator with 1000Hz square wave
  /* Throttle Motor */
  motor.init();
  /* Steering Wheel */
  wheel.init();
  /* Brake Actuator*/
  brake.init();

  calibrationTest();
}

void loop() {

}

void calibrationTest() {
  Serial.println("Running Go Kart Calibration Test.");

  /* sweep motor rpms */
  Serial.println("Testing Throttle Motor...");
  gradient = 1;
  throttle_pwm = (throttle_pwm + gradient) % (motor.MAX_THROTTLE + 1);
  while(throttle_pwm != motor.MIN_THROTTLE) { 
    if(throttle_pwm == motor.MAX_THROTTLE) gradient = -1;
    motor.setThrottle(throttle_pwm);
    Serial.println(throttle_pwm);
    delay(25);
    throttle_pwm = (throttle_pwm + gradient) % (motor.MAX_THROTTLE + 1);
  }
  
  delay(250);

  /* sweep steering angles */
  Serial.println("Testing Steering Falcon 500 Motor...");
  gradient = 1;
  steering_theta = (steering_theta + gradient) % (wheel.MAX_STEER + 1);
  while(steering_theta != wheel.MIN_STEER) {
    if(steering_theta == wheel.MAX_STEER) gradient = -1;
    wheel.setSteer(steering_theta);
    Serial.println(steering_theta);
    delay(25);
    steering_theta = (steering_theta + gradient) % (wheel.MAX_STEER + 1);
  }

  delay(250);

  /* sweep brake accuator */
  Serial.println("Testing KarTech Brake Actuator...");
  gradient = 1;
  actuator_position = (actuator_position + gradient) % (brake.MAX_POSITION + 1);
  while(actuator_position != brake.MIN_POSITION) {
    if(actuator_position == brake.MAX_POSITION) gradient = -1;
    brake.setPosition(actuator_position);
    Serial.println(actuator_position);
    delay(25);
    actuator_position = (actuator_position + gradient) % (brake.MAX_POSITION + 1);
  }
}
