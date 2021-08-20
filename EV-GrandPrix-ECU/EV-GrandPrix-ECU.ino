/**
 * Jose Jimenez-Olivas 
 * Jesus Fausto
 * Dallas Domingues
 * Email: jjj023@ucsd.edu
 * 
 *                 University of California, San Diego
 *                             EV-GrandPrix
 *
 * File Name:   EV-GrandPrix-ECU.ino
 * Description: Remote controlled Go-Kart.
 * Sources of Help: https://github.com/jimenezjose/EV-GrandPrix/wiki
 */
#include <SPI.h>
#include "mcp2515_can.h" // CAN shield

#include "Motor.h"
#include "SteeringWheel.h"
#include "BrakeActuator.h"

// Set SPI CS Pin according to your hardware
// For Arduino MCP2515 Hat:
const int SPI_CS_PIN = 9;

const int safetyPin = 6; 
const int altraxThrottlePin = 5; // OK
const int pedalPin = 4;
const int pedalSwitch = 3;
const int steerMinSwitch = 7;
const int steerMaxSwitch = 8;

mcp2515_can CAN(SPI_CS_PIN); 
Motor motor(altraxThrottlePin);
SteeringWheel wheel(steerMinSwitch, steerMaxSwitch);
BrakeActuator brake(0x00FF0302, CAN);

void setup() {
  Serial.begin(9600);
  while(!Serial){};

  // Set CAN baudrate to 250k
  while (CAN_OK != CAN.begin(CAN_250KBPS)) {
        Serial.println("CAN init fail, retry...");
        delay(100);
  }
  Serial.println("CAN init ok!");

  /* Physical Pedal */
  pinMode(pedalPin, INPUT);
  pinMode(pedalSwitch, INPUT);
  /* Pedal Emulator */
  pinMode(safetyPin, OUTPUT);
  analogWrite(safetyPin, 127); // enable pedal emulator (DAC) with 1000Hz square wave
  /* Throttle Motor */
  motor.init();
  /* Steering Wheel */
  wheel.init();
  /* Brake Actuator*/
  brake.init();

 calibrationTest();
}

void loop() {
  // controllerTest();
}

// demonstrate control of throttle, steering wheel, and brake.
void calibrationTest() {
  Serial.println("Running Go Kart Calibration Test.");
  /* sweep motor rpms */
  motor.test();
  delay(250);
  /* sweep steering angles */
  wheel.test();
  delay(250);
  /* sweep brake accuator */
  brake.test();
  delay(250);
  Serial.println("Done.");
}

// parse bluetooth data and map it to the control button functions
void controllerTest() {
  String commandName[] = {"left", "up", "down", "select", "start", "square", "triangle", "x", "circle"};
  void (* commandFunc[])() = {left, up, right, down, select, start, square, triangle, x, circle};
  if(Serial.available() > 0) {
    int index = Serial.read() - '0';
    Serial.println(commandName[index]);
    commandFunc[index]();
  }
}

// Control Steering
int steer_angle = wheel.MIN_ANGLE;
int steer_gradient = 12;
void left() {
  if(steer_angle <= wheel.MIN_ANGLE) steer_angle = wheel.MIN_ANGLE;
  else steer_angle -= steer_gradient;
  wheel.setSteer(steer_angle);
}
void right() {
  if(steer_angle >= wheel.MAX_ANGLE) steer_angle = wheel.MAX_ANGLE;
  else steer_angle += steer_gradient;
  wheel.setSteer(steer_angle);
}

// Control Throttle
int throttle = motor.MIN_THROTTLE;
int throttle_gradient = 25;
void up() {
  if(throttle >= motor.MAX_THROTTLE) throttle = motor.MAX_THROTTLE;
  else throttle += throttle_gradient;
  motor.setThrottle(throttle); 
}
void down() {
  if(throttle <= motor.MIN_THROTTLE) throttle = motor.MIN_THROTTLE;
  else throttle -= throttle_gradient;
  motor.setThrottle(throttle);
}

// Control Brake
void square() {
  brake.setPosition(brake.MAX_POSITION);
}
void circle() {
  brake.setPosition(brake.MIN_POSITION);
}

void triangle() {}
void x() {}
void select() {}
void start() {}
