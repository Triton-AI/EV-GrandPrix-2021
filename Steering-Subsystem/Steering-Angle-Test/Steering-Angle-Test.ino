/*  
 *   This code calibrates the steering wheel to be centered using the limit switches 
 *   After calibration it calculates the angle in which it currently is using the potentiometer
 */
// Limit switch pins
const int leftButton = 2;         // the number of the right limit pin
const int rightButton = 4;        // the number of the left limit pin

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

// Takes the angle using the potentiometer reading, comparing it to the max and min potentiometer positions
//  and comparing them to the max and min possible angles
double calculateAngle(int currRead){
  int relativeAngle = maxAngle-minAngle;
  int totalRead = maxRight-maxLeft;
  return double(currRead-maxLeft)*(double(relativeAngle)/double(totalRead))+minAngle;
}
// Turns the wheel completely to one side, then turn it completely in the other direction
// The code will keep track of both ends and relate them to the max position
void calibrateSteering(){
    while(maxLeft == -1 || maxRight == -1){
      limitLeft = digitalRead(leftButton);
      limitRight = digitalRead(rightButton);
    
      // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
      if (limitLeft == HIGH && maxLeft == -1) {
        maxLeft = analogRead(potentiometerPin);
        Serial.println("on the left");
      } 
      else if(limitRight == HIGH && maxRight == -1) {
        maxRight = analogRead(potentiometerPin);
        Serial.println("on the right");
      }
      else {
        Serial.println("on the middle");
      }
      delay(100);
  }
}
void setup() {
  Serial.begin(9600);
  calibrateSteering();
}

void loop() {
  //prints the expected steering value
  val = analogRead(potentiometerPin);  // read the input pin
  Serial.print("Angle ");
  Serial.println(calculateAngle(val));          // debug value
  delay(100);
}
