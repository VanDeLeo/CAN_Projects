#include <mcp_can.h>
#include <SPI.h>

const int SPI_CS_PIN = 10;
MCP_CAN CAN(SPI_CS_PIN);

const unsigned long ID_REQUEST = 0x7E0; //CAN ID for request

const unsigned long ID_RESPONSE = 0x7E8; //CAN ID for response

void sendUDS(byte* data, byte len, const char* description) {
  CAN.sendMsgBuf(ID_REQUEST, 0, len, data);
  Serial.print("Sent → ");
  Serial.print(description);
  Serial.print(": ");
  for (int i = 0; i < len; i++)
  {
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);

  while (CAN_OK != CAN.begin(CAN_500KBPS)) {
    Serial.println("Waiting for CAN bus...");
    delay(100);
  }
  Serial.println("CAN bus initialized.");

  // UDS_Sim: Diagnostic Session Control (0x10)
  byte sessionControl[] = {0x02, 0x10, 0x01}; // 0x02 = request, 0x10 = service ID, 0x01 = default session
  sendUDS(sessionControl, sizeof(sessionControl), "Diagnostic Session Control");

  delay(500);

}

void loop() {

  //Tester Present Sim
  if (millis() - lastTesterPresent > testterPresentInterval)
  {
    byte testerPresent[] = {0x02, 0x3E, 0x00}; // Tester Present request
    sendUDS(testerPresent, sizeof(testerPresent), "Tester Present");
    lastTesterPresent = millis();
  }

  if (CAN.checkReceive() == CAN_MSGAVAIL) {
    long unsigned long rxID;
    byte len = 0;
    byte rxBuf[8];

    CAN.readMsgBuf(&rxID, &len, rxBuf);

    if (rxID == ID_RESPONSE) {
      Serial.print("Received ← ");
      Serial.print("ID: ");
      Serial.print(rxID, HEX);
      Serial.print(", Data: ");
      for (int i = 0; i < len; i++)
      {
        Serial.print(rxBuf[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
    } 
  }  
}
