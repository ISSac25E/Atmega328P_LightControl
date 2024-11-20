#define DMX_FIXTURE_CNT 9
#define DMX_FIXT_LIST {1, 33, 65, 97, 129, 161, 193, 225, 257}
#define DMX_FIXT_CHNL 5

#include "DMX_Driver.h"
#include "UDP_Parse.h"

byte ARD_MAC[] = {0x44, 0x10, 0x42, 0x83, 0x20, 0x93};
IPAddress ARD_IP(192, 168, 86, 179);

UDP_Parse UDP;

DMX_Driver dmx(3);

void setup() {
  Ethernet.begin(ARD_MAC, ARD_IP);
  UDP.Begin(7575);
  Serial.begin(115200);
  Serial.println("INIT");
}

void loop() {
  while (UDP.UDP_Avail()) {
    char Buff[20];
    uint8_t Bytes = UDP.ParseCMD(Buff);
    if (Bytes >= 4) {
      char CmdStr[] = {
        Buff[0], Buff[1],
        Buff[2], Buff[3], '\0'
      };
      if (!strcmp(CmdStr, "FxSt")) {
        Serial.println("FxSt");
        uint8_t ParseIndex = 5;
        uint8_t Fixt = ParseInt(Buff, ParseIndex, Bytes);
        ParseIndex++;
        uint8_t Chnl = ParseInt(Buff, ParseIndex, Bytes);
        ParseIndex++;
        uint8_t Val = ParseInt(Buff, ParseIndex, Bytes);
        Serial.print(Fixt); Serial.print(' ');
        Serial.print(Chnl); Serial.print(' ');
        Serial.println(Val);
        dmx.Write(Fixt, Chnl, Val);
      }
      if (!strcmp(CmdStr, "FxHx")) {
        Serial.println("FxHx");
        uint8_t ParseIndex = 5;
        uint8_t Fixt = ParseInt(Buff, ParseIndex, Bytes);
        ParseIndex++;
        uint8_t Chnls[5];
        for(uint8_t X = 0; X < 5; X++) Chnls[X] = ParseHex(Buff, ParseIndex, Bytes);
        Serial.print(Fixt); Serial.print(' ');
        for(uint8_t X = 0; X < 5; X++) {
          Serial.print(Chnls[X]); Serial.print(' ');
        }
        Serial.println();
        dmx.Write(Fixt, Chnls);
      }
    }
  }
  dmx.Run();
}

uint16_t ParseInt(char *Data, uint8_t &Index, uint8_t Max) {
  uint16_t Answer = 0;
  while (Data[Index] >= '0' && Data[Index] <= '9' && Index < Max) {
    Answer = (10 * Answer) + (Data[Index] - '0');
    Index++;
  }
  return Answer;
}

uint8_t ParseHex(char *Data, uint8_t &Index, uint8_t Max) {
  uint16_t Answer = 0;
  bool RunCnt = false;
  while (((Data[Index] >= '0' && Data[Index] <= '9') || (Data[Index] >= 'A' && Data[Index] <= 'F')) && Index < Max) {
    if (Data[Index] >= 'A') {
      Answer = (16 * Answer) + (Data[Index] - 55);
      Index++;
    }
    else {
      Answer = (16 * Answer) + (Data[Index] - '0');
      Index++;
    }
    if(RunCnt) return Answer;
    else RunCnt = true;
  }
  return Answer;
}
