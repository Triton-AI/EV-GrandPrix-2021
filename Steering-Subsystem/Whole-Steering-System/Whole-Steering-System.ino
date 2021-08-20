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
int minAngle = 0;
int maxAngle = 180;

const int leftFreq = 332;
const int rightFreq = 328;



//creates pwm object
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver();

// Takes the angle using the potentiometer reading, comparing it to the max and min potentiometer positions
//  and comparing them to the max and min possible angles
double calculateAngle(int currRead){
  int relativeAngle = maxAngle-minAngle;
  int totalRead = maxRight-maxLeft;
  return double(currRead-maxLeft)*(double(relativeAngle)/double(totalRead))+minAngle;
}

// Turns the wheel completely to one side, then turn it completely in the other direction
// The code will keep track of both ends and relate them to estimate angle
void calibrateSteering(){
    pwm1.setPWM(0, 3072, 1024 );
    while(maxLeft == -1 || maxRight == -1){
      limitLeft = digitalRead(leftButton);
      limitRight = digitalRead(rightButton);
    
      // check if the limit switch is pressed.
      // once it is store that position and move on
      if (limitLeft == HIGH && maxLeft == -1) {
        pwm1.setPWMFreq(rightFreq);
        maxLeft = analogRead(potentiometerPin);
        Serial.println("on the left");
      } 
      else if(limitRight == HIGH && maxRight == -1) {
        maxRight = analogRead(potentiometerPin);
        pwm1.setPWMFreq(leftFreq);
        Serial.println("on the right");
      }
      else {
        Serial.println("on the middle");
      }
      delay(10);
  }
}

//Moves from current angle to decired angle
void getToAngle(int deciredAngle){
  double currentAngle;
  
  pwm1.setPWM(0, 3072, 1024 );
  val = analogRead(potentiometerPin);  // read the input pin
  currentAngle = calculateAngle(val);
  Serial.print("current angle ");
  Serial.println(currentAngle);
  
  //move left or right depending on the current angle
  if(currentAngle-deciredAngle<0) pwm1.setPWMFreq(rightFreq);
  else pwm1.setPWMFreq(leftFreq);

  //keep on going torwards that angle when you are outside of 5 degrees
  while(deciredAngle+5<currentAngle || currentAngle<deciredAngle-5){
      val = analogRead(potentiometerPin);  // read the input pin
      currentAngle = calculateAngle(val);
      Serial.print("current angle ");
      Serial.println(currentAngle);

      //Ensures we don't overshoot with limit switches
      if (limitLeft == HIGH) {
        pwm1.setPWMFreq(rightFreq);
      } 
      else if(limitRight == HIGH) {
        pwm1.setPWMFreq(leftFreq);
      }
      
  }
  pwm1.setPWM(0,0,4096);
}

void setup() {
  pwm1.begin();
  pwm1.setPWMFreq(332);
  pinMode(3,OUTPUT);
  Serial.begin(9600);
  calibrateSteering();
  pwm1.setPWM(0, 0, 4096 );
  Serial.print("after calibration");
}
void loop() {
    //prints the expected steering value
  while(Serial.available() == 0){
    
  }
  int deciredAngle = Serial.parseInt();
  Serial.print("going to angle ");
  Serial.println(deciredAngle);  
  getToAngle(deciredAngle);
  
  //get rid of trash values
  Serial.flush();
  deciredAngle = Serial.parseInt();
  
}
