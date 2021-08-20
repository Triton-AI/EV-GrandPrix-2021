/*  
 *   This code calibrates the steering wheel to be centered using the limit switches 
 *   After calibration it calculates the angle in which it currently is using the potentiometer
 */
 
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Limit switch pins
const int leftButton = 2;         // the number of the right limit pin
const int rightButton = 18;        // the number of the left limit pin

// variables to keep track wether the limit switches have been pressed
int limitLeft = 0;                
int limitRight = 0;               

int potentiometerPin = A0;        // pin to read potentiometer value
int val = 0;

// max left and right steering position
int maxLeft = -1;                 
int maxRight = -1;

// max possible angle in the steering wheel
int minAngle = -90;
int maxAngle = 90;

int currentAngle;
int currFreq = 332;
int prefFreq;
// Takes the angle using the potentiometer reading, comparing it to the max and min potentiometer positions
//  and comparing them to the max and min possible angles

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver();

void setup() {
  pwm1.begin();
  pwm1.setPWMFreq(332);
  pinMode(3,OUTPUT);
  Serial.begin(9600);
  //calibrateSteering();
  pwm1.setPWM(0, 3072, 1024 );
}
void loop() {
  //this code sweeps the motor from end to end
  limitLeft = digitalRead(leftButton);
  limitRight = digitalRead(rightButton);
  if (limitLeft == HIGH) {
      pwm1.setPWMFreq(327);
      Serial.print("on left");
  } 
  else if(limitRight == HIGH) {
      pwm1.setPWMFreq(332);
      Serial.print("on right");

  }
  delay(50);
}
