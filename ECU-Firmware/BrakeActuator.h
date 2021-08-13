#ifndef BRAKE_ACTUATOR_H
#define BRAKE_ACTUATOR_H

#include "mcp2515_can.h"

class BrakeActuator {
private:
  // Data Format: (8 bytes)
  // | Byte 0 | Byte 1        | Byte 2   | Byte 3           | Byte 4-7
  //  _________________________________________________________________
  // | 0x0F   | C / A / DT=10 | DPOS_LOW | CE / M / DPOS_HI | ...     | 
  //  _________________________________________________________________
  unsigned char CAN_DATA[8] = {0x0F, 0x4A, 0xC4, 0xC9, 0, 0, 0, 0};
  unsigned char CAN_DATA_LENGTH = 8;

public:
  const int MAX_POSITION = 3450;
  const int MIN_POSITION = 550;
  const mcp2515_can CAN;
  const int32_t CAN_ID;
    
  BrakeActuator(int32_t can_id, mcp2515_can can) : CAN_ID(can_id), CAN(can) {}

  void setPosition(int pos);
  void test();

  void init() {}

};

void BrakeActuator::setPosition(int pos) {
  // Note every unit is 0.001"
  CAN_DATA[2] = pos & 0xFF;
  CAN_DATA[3] = (CAN_DATA[3] & 0xC0) | ((pos >> 8) & 0x1F);
  CAN.sendMsgBuf(CAN_ID, CAN_EXTID, CAN_DATA_LENGTH, CAN_DATA); 
  return;
}

void BrakeActuator::test() {
  Serial.println("Testing KarTech Brake Actuator...");
  int gradient = 50;
  int actuator_position = (MIN_POSITION + gradient) % (MAX_POSITION + 1);
  while(actuator_position != MIN_POSITION) {
    if(actuator_position == MAX_POSITION) gradient = -50;
    setPosition(actuator_position); 
    Serial.println(actuator_position);
    delay(100);
    actuator_position += gradient;
  }
  setPosition(actuator_position); 
}


/**
Idea: 

The CAN brake actuator disengages the clutch after not recieving a command within 1second.
The actuator datasheet recommends a 100ms command refresh rate.

It is important that when the user wants say a 50% brake output the brake stays at 50%
until the user specifies a different value. The brake should stay engaged on the last 
command sent to it for its lifetime. 

A way to implement a 100ms refresh rate is by using timer interrupts using one of the three
timers that the avr microcontroller on the arduino unos have. Here is the code to enable this
feature: (Note this feature is important for emergency brakes.)


Other Ideas:
The timer interupts should be handled in the brake actuator class to hide the detail implementation
necessary for the actuator to work as expected. The user expects to call a function brake.setPosition(x)
and have the brake actuator be set to a distance of x for the lifetime of the program until that function
is invoked again where the new value will overwrite the position of the actuator.

On the other hand, hidding the timer interupts makes it not scalable if other use cases require
the use of timer interupts. Even though there are 3. timer1 uses an 16bit counter instead of an 8bit.
The 16 bit counter allows the user to have a slower trigger frequency that would allow interupts for as slow
as 10Hz instead of the arduino clock frequency of 16MHz.

void init() {
      // hardware interup every 100ms = 10Hz
    cli();//stop interrupts
    //set timer1 interrupt at 10Hz
    TCCR1A = 0; // set entire TCCR1A register to 0
    TCCR1B = 0; // same for TCCR1B
    TCNT1  = 0; //initialize counter value to 0
    // set compare match register for 10hz increments
    OCR1A = 1562; // = (16*10^6) / (10*1024) - 1 (must be < 65536)
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    // Set CS10 and CS12 bits for 1024 prescaler
    TCCR1B |= (1 << CS12) | (1 << CS10);  
    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);
    sei();//allow interrupts
}

ISR(TIMER1_COMPA_vect){ //timer1 interrupt 10Hz
  Serial.println("Hello I am triggered at 10Hz");
}

 */

#endif
