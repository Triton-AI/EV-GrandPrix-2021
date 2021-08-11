// demo: CAN-BUS Shield, send data
// loovee@seeed.cc


#include <SPI.h>

#define CAN_2515
// #define CAN_2518FD

// Set SPI CS Pin according to your hardware

#if defined(SEEED_WIO_TERMINAL) && defined(CAN_2518FD)
// For Wio Terminal w/ MCP2518FD RPi Hat：
// Channel 0 SPI_CS Pin: BCM 8
// Channel 1 SPI_CS Pin: BCM 7
// Interupt Pin: BCM25
const int SPI_CS_PIN  = BCM8;
const int CAN_INT_PIN = BCM25;
#else

// For Arduino MCP2515 Hat:
// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;
const int CAN_INT_PIN = 2;
#endif


#ifdef CAN_2518FD
#include "mcp2518fd_can.h"
mcp2518fd CAN(SPI_CS_PIN); // Set CS pin
#endif

#ifdef CAN_2515
#include "mcp2515_can.h"
mcp2515_can CAN(SPI_CS_PIN); // Set CS pin
#endif

void setup() {
    SERIAL_PORT_MONITOR.begin(115200);
    while(!Serial){};

    while (CAN_OK != CAN.begin(CAN_250KBPS)) {             // init can bus : baudrate = 500k
        SERIAL_PORT_MONITOR.println("CAN init fail, retry...");
        delay(100);
    }
    SERIAL_PORT_MONITOR.println("CAN init ok!");
}

// C|A|DT DPOS_LOW CE|M|DPOS_HI
unsigned char enable_clutch[8] = {0x0F, 0x4A, 0xC4, 0x89, 0, 0, 0, 0}; // 0b1000 1001 // enable clutch
unsigned char enable_clutch_motor[8] = {0x0F, 0x4A, 0xC4, 0xC9, 0, 0, 0, 0}; // 0b1100 1001 // enable motor and clutch

void loop() {

//    CAN.sendMsgBuf(0x00FF0302, CAN_EXTID, 8, enable_clutch); // 2''
//    delay(20);
  int gradient = 50;
  int pos = 550 + gradient;
  Serial.println(pos);
  while(pos != 550) {
    if(pos == 3450) gradient = -50;
    enable_clutch_motor[2] = pos & 0xFF;
    enable_clutch_motor[3] = (enable_clutch_motor[3] & 0xC0) | ((pos >> 8) & 0x1F);
    int DPOS = (enable_clutch_motor[3] & 0x3F) << 8 | enable_clutch_motor[2];
    Serial.println(DPOS);
    CAN.sendMsgBuf(0x00FF0302, CAN_EXTID, 8, enable_clutch_motor); // 2''
    delay(100);                       // send data per 100ms
    pos += gradient;
  }
  SERIAL_PORT_MONITOR.println("CAN BUS sendMsgBuf ok!");
  delay(5000);
}

// END FILE
