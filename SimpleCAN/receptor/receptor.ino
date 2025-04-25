#include <mcp_can.h>
#include <SPI.h>

const int SPI_CS_PIN = 10;

MCP_CAN CAN(SPI_CS_PIN);

void setup() {
  Serial.begin(9600);

  Serial.println("MCP2515 CAN Bus Receiver Test");
  
  if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println("CAN init ok!");
  } else {
    Serial.println("CAN init fail");
    while (1);
  }
  CAN.setMode(MCP_NORMAL);
}

void loop() {
  if (CAN.checkReceive() == CAN_MSGAVAIL) {
    unsigned long rxId = 0;
    unsigned char len = 0;
    unsigned char buf[8];
    
    CAN.readMsgBuf(&rxId, &len, buf);
    
    Serial.print("ID: ");
    Serial.print(rxId, HEX);
    Serial.print(" Data: ");
    for (int i = 0; i < len; i++) {
      Serial.print(buf[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  } 
  delay(10);
}

