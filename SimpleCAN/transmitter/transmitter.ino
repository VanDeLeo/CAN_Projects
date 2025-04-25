#include <mcp_can.h>
#include <SPI.h>

const int SPI_CS_PIN = 10;

MCP_CAN CAN(SPI_CS_PIN);

void setup() {
  Serial.begin(9600);

  // Initialize the CAN bus

  Serial.println("MCP2515 CAN Bus Transmitter Test");
  
  if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println("CAN init ok!");
  } else {
    Serial.println("CAN init fail");
    while (1);
  }
  CAN.setMode(MCP_NORMAL);
}

void loop() {

  // Send a message with ID 0x100 and 8 bytes of data
  unsigned char data[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

  if (CAN.sendMsgBuf(0x100, 0, 8, data) == CAN_OK) {
    Serial.println("Message Sent");
  } else {
    Serial.println("Message Failed");
  }

  delay(1000);
}

