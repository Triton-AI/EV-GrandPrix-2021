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

#include <SPI.h>
#include "mcp2515_can.h" // CAN shield

#include "Motor.h"
#include "SteeringWheel.h"
#include "BrakeActuator.h"

// Set SPI CS Pin according to your hardware
// For Arduino MCP2515 Hat:
const int SPI_CS_PIN = 9;

const int safetyPin = 6; 
const int altraxThrottlePin = 5;
const int pedalPin = 4;
const int steerMinSwitch = 7;
const int steerMaxSwitch = 8;

mcp2515_can CAN(SPI_CS_PIN); 
Motor motor(altraxThrottlePin);
SteeringWheel wheel(steerMinSwitch, steerMaxSwitch);
BrakeActuator brake(0x00FF0302, CAN);

void setup() {
  Serial.begin(115200);
  while(!Serial){};

  // Set CAN baudrate to 250k
  while (CAN_OK != CAN.begin(CAN_250KBPS)) {
        Serial.println("CAN init fail, retry...");
        delay(100);
  }
  Serial.println("CAN init ok!");

  /* Physical Pedal */
  pinMode(pedalPin, INPUT);
  /* Pedal Emulator */
  pinMode(safetyPin, OUTPUT);
  analogWrite(safetyPin, 127); // enable pedal emulator with 1000Hz square wave
  /* Throttle Motor */
  motor.init();
  /* Steering Wheel */
//  wheel.init();
  /* Brake Actuator*/
  brake.init();

  calibrationTest();
}

void loop() {

}

void calibrationTest() {
  Serial.println("Running Go Kart Calibration Test.");
  /* sweep motor rpms */
  motor.test();
  delay(250);
  /* sweep steering angles */
  //wheel.test();
  //delay(250);
  /* sweep brake accuator */
  brake.test();
  delay(250);
}
