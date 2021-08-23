#ifndef STEERING_WHEEL_H
#define STEERING_WHEEL_H

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

class SteeringWheel {
private:

public:
    const int leftFreq = 332;
    const int rightFreq = 328;
    const int minSwitch;
    const int maxSwitch;
    const int potentiometerPin;
    const int MIN_ANGLE = -90;
    const int MAX_ANGLE = 90;
    int maxLeft = -1;
    int maxRight = -1;

    Adafruit_PWMServoDriver falconPWM;
    
    // variables to keep track wether the limit switches have been pressed
    int limitLeft = 0;                
    int limitRight = 0;     
    
    SteeringWheel(int switchA, int switchB, int aPin, Adafruit_PWMServoDriver aPWM) : 
        minSwitch(switchA), maxSwitch(switchB) , potentiometerPin(aPin), falconPWM(aPWM){}
    

    
    // Takes the angle using the potentiometer reading, comparing it to the max and min potentiometer positions
    //  and comparing them to the max and min possible angles
    double calculateAngle(int currRead){
      int relativeAngle = MAX_ANGLE-MIN_ANGLE;
      int totalRead = maxRight-maxLeft;
      return double(currRead-maxLeft)*(double(relativeAngle)/double(totalRead))+MIN_ANGLE;
    }

    void init(){
      falconPWM.begin();
      falconPWM.setPWMFreq(rightFreq);

      }
    // Turns the wheel completely to one side, then turn it completely in the other direction
    // The code will keep track of both ends and relate them to estimate angle
    void calibrateSteering(){
        falconPWM.setPWM(0, 3072, 1024 );
        while(maxLeft == -1 || maxRight == -1){
          limitLeft = digitalRead(minSwitch);
          limitRight = digitalRead(maxSwitch);
        
          // check if the limit switch is pressed.
          // once it is store that position and move on
          if (limitLeft == HIGH && maxLeft == -1) {
            falconPWM.setPWMFreq(rightFreq);
            maxLeft = analogRead(potentiometerPin);
            Serial.println("on the left");
          } 
          else if(limitRight == HIGH && maxRight == -1) {
            maxRight = analogRead(potentiometerPin);
            falconPWM.setPWMFreq(leftFreq);
            Serial.println("on the right");
          }
          else {
            Serial.println("on the middle");
          }
          delay(10);
      }
      setSteer(0);
      falconPWM.setPWM(0, 0, 4096 );

    }
    
    //Moves from current angle to decired angle
    void setSteer(int deciredAngle){
      double val;
      double currentAngle;
      
      falconPWM.setPWM(0, 3072, 1024 );
      val = analogRead(potentiometerPin);  // read the input pin
      currentAngle = calculateAngle(val);
      Serial.print("current angle ");
      Serial.println(currentAngle);
      
      //move left or right depending on the current angle
      if(currentAngle-deciredAngle<0) falconPWM.setPWMFreq(327);
      else falconPWM.setPWMFreq(333);
    
      //keep on going torwards that angle when you are outside of 5 degrees
      while(deciredAngle+5<currentAngle || currentAngle<deciredAngle-5){
          val = analogRead(potentiometerPin);  // read the input pin
          currentAngle = calculateAngle(val);
          Serial.print("current angle ");
          Serial.println(currentAngle);
    
          limitLeft = digitalRead(minSwitch);
          limitRight = digitalRead(maxSwitch);
          //Ensures we don't overshoot with limit switches
          if (limitLeft == HIGH) {
            falconPWM.setPWMFreq(rightFreq);
          } 
          else if(limitRight == HIGH) {
            falconPWM.setPWMFreq(leftFreq);
          }
          
      }
      falconPWM.setPWM(0,0,4096);
    }
    
    void SteeringWheel::test() {
      calibrateSteering();
    }
};


#endif
