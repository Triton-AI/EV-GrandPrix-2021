//This is a basic steering potentiometer
// white is signal
// black is ground
// blue is VCC(reference voltage, 5V in arduino)
// black and blue can be flipped if desidred
int white = A0;
int val = 0;
void setup() {
  Serial.begin(9600);

}

void loop() {
  val = analogRead(white);  // read the input pin
  Serial.print("white ");
  Serial.println(val);          // debug value
  delay(100);
}
