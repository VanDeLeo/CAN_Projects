#include <mcp_can.h>
#include <SPI.h>

const int SPI_CS_PIN = 10;
MCP_CAN CAN(SPI_CS_PIN);

const unsigned long ID_REQUEST = 0x7E0; //CAN ID for request
const unsigned long ID_RESPONSE = 0x7E8; //CAN ID for response

char vin[] = "VIN12345678";
uint16_t RPM = 3000;
uint8_t session = 0x01; // Default session

void setup() { 
    Serial.begin(115200);
    while (CAN_OK != CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ)) {
        Serial.println("Waiting for CAN bus...");
        delay(100);
    }
    Serial.println("CAN bus initialized.");
}

void sendResponse(byte* data, byte len) {
    CAN.sendMsgBuf(ID_RESPONSE, 0, len, data);
    Serial.print("Sent ← ");
    for (int i = 0; i < len; i++) {
        Serial.print(data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

void sendNegativeResponse(byte service, byte errorCode) {
    byte response[] = {0x03, 0x7F, service, errorCode};
    sendResponse(response, sizeof(response));
}

void loop() {
    if (CAN.checkReceive() == CAN_MSGAVAIL)
    {
        unsigned long rxID;
        byte len = 0;
        byte rxBuf[8];
        byte service = rxBuf[1];
        CAN.readMsgBuf(&rxID, &len, rxBuf);

        if (rxID != ID_REQUEST) {
            return; // Ignore messages not meant for us
        }

        switch (service) {
            // case 0x10: {
            //     session = rxBuf[2]; // Set session type
            //     byte response[] = {0x02, 0x50, session}; // Positive response
            //     sendResponse(response, sizeof(response));
            //     break;
            // }
            // case 0x22: {
            //     if (rxBuff[2] == 0xF1 && rxBuff[3] == 0x90) {
            //         byte response[] = {0x10,0x14,0x62,0x62,0xF1,0x90};
            //         CAN.sendMsgBuf(ID_RESPONSE, 0, sizeof(response), response);
            //         delay(10);
            //         byte vinData[] = {'V','I','N'};
            //         CAN.sendMsgBuf(ID_RESPONSE, 0, sizeof(vinData), vinData);
            //         delay(10);
            //         byte vinEnd[] = {'1','2','3','4','5','6','7','8'};
            //         CAN.sendMsgBuf(ID_RESPONSE, 0, sizeof(vinEnd), vinEnd);
            //     } else {
            //         sendNegativeResponse(service, 0x31); // Request out of range
            //     }               
            // }
            case 0x3E: {
                byte response[] = {0x02, 0x7E, 0x00}; // Tester Present response
                sendResponse(response, sizeof(response));
                break;
            }
            default: {
                sendNegativeResponse(service, 0x11); // General Reject
                break;
            }
        }
    }
    
}