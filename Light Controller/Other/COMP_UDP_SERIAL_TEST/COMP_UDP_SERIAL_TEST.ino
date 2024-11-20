#include <SPI.h>
#include <UIPEthernet.h>
#include <UIPUdp.h>

EthernetUDP COMP_TX;

byte ARD_MAC[] = {0x98, 0xA6, 0xAA, 0x15, 0xE8, 0xD9};
IPAddress ARD_IP(192, 168, 86, 177);

IPAddress COMP_IP(192, 168, 86, 138);

void setup() {
  // put your setup code here, to run once:
  Ethernet.begin(ARD_MAC, ARD_IP);
  COMP_TX.begin(2539);
  Serial.begin(115200);
//  const char C[] = "BANK-PRESS 1 3";
//    COMP_TX.beginPacket(COMP_IP, 51237);
//    COMP_TX.write(C);
//    COMP_TX.endPacket();
}

void loop() {

//  const char C[] = "BANK-PRESS 1 3";
//    COMP_TX.beginPacket(COMP_IP, 51237);
//    COMP_TX.write(C, 14);
//    COMP_TX.endPacket();
//    delay(500);


  if (Serial.available()) {
    char C[255];
    uint8_t C_Cnt = 0;

    while (Serial.available()) {
      if (Serial.peek() != '\n') {
        C[C_Cnt] = Serial.read();
        C_Cnt++;
      }
      else Serial.read();
      delay(2);
    }
    for (uint16_t X = 0; X < C_Cnt; X++) Serial.write(C[X]);
    Serial.println();
    COMP_TX.beginPacket(COMP_IP, 51237);
    COMP_TX.write(C, C_Cnt);
    COMP_TX.endPacket();

  }
}
