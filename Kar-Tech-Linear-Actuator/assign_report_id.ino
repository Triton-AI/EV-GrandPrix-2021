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

// This message is used to configure the User Defined Report CAN ID
// Here we set the USER DEFINED REPORT ID to 0x00FF0201, and select
// the USER DEFINED REPORT ID as the active Report ID
unsigned char report_id[8] = {0xF7, 0x00, 0x01, 0x02, 0xFF, 0x00, 0x01, 0x00};

void setup() {
    SERIAL_PORT_MONITOR.begin(115200);
    while(!Serial){};

    while (CAN_OK != CAN.begin(CAN_250KBPS)) {             // init can bus : baudrate = 500k
        SERIAL_PORT_MONITOR.println("CAN init fail, retry...");
        delay(100);
    }
    SERIAL_PORT_MONITOR.println("CAN init ok!");
   
    //Report ID assignment
    //Message 1
    CAN.sendMsgBuf(0x00FF0302, CAN_EXTID, 8, report_id);
    delay(100);                       // send data per 100ms
}



void loop() {
//    // send data:  id = 0x00, standrad frame, data len = 8, stmp: data buf

    SERIAL_PORT_MONITOR.println("CAN BUS sendMsgBuf ok!");
}

// END FILE
