#define DMX_FIXTURE_CNT 9
#define DMX_FIXT_LIST {1, 33, 65, 97, 129, 161, 193, 225, 257}
#define DMX_FIXT_CHNL 5

#define VarMacro_BYTES (DMX_FIXTURE_CNT * DMX_FIXT_CHNL)

#include "VarMacro.h"
#include "DMX_Driver.h"
#include "UDP_Parse.h"
#include <EEPROM.h>

byte ARD_MAC[] = {0x44, 0x10, 0x42, 0x83, 0x20, 0x93};
IPAddress ARD_IP(192, 168, 86, 179);

uint8_t DMX_BUFF[DMX_FIXTURE_CNT * DMX_FIXT_CHNL];
uint8_t DMX_TAR_BUFF[DMX_FIXTURE_CNT * DMX_FIXT_CHNL];

bool EEPROM_Writing = false;
uint8_t EEPROM_WriteMacro;
uint8_t EEPROM_FixtWrite[2] = {0, 0};

UDP_Parse UDP;
DMX_Driver Dmx(3, DMX_BUFF);
VarMacro DmxMacro(DMX_BUFF, DMX_TAR_BUFF);

void setup() {
  Ethernet.begin(ARD_MAC, ARD_IP);
  Serial.begin(115200);
  Serial.println("INIT");
  UDP.Begin(7575);

  LoadMacro(0);
  DmxMacro.RunMacro((EEPROM.read(1000) | (EEPROM.read(1001) << 8)));
}

void loop() {
  while (UDP.UDP_Avail()) {
    char Buff[40];
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
        DMX_BUFF[Dmx.ChnlAdd(Fixt, Chnl)] = Val;
        DmxMacro.StopMacro();
      }
      if (!strcmp(CmdStr, "FxHx")) {
        Serial.println("FxHx");
        uint8_t ParseIndex = 5;
        uint8_t Fixt = ParseInt(Buff, ParseIndex, Bytes);
        ParseIndex++;
        uint8_t Chnls[5];
        for (uint8_t X = 0; X < 5; X++) {
          Chnls[X] = ParseHex(Buff, ParseIndex, Bytes);
          DMX_BUFF[Dmx.ChnlAdd(Fixt, X)] = Chnls[X];
        }
        Serial.print(Fixt); Serial.print(' ');
        for (uint8_t X = 0; X < 5; X++) {
          Serial.print(Chnls[X]); Serial.print(' ');
        }
        Serial.println();
        DmxMacro.StopMacro();
      }
      if (!strcmp(CmdStr, "SrMc")) {
        Serial.print("SrMc ");
        uint8_t ParseIndex = 5;
        EEPROM_WriteMacro = ParseInt(Buff, ParseIndex, Bytes);
        Serial.print(EEPROM_WriteMacro);
        Serial.print(" ");
        ParseIndex++;
        if (EEPROM_WriteMacro < 20 && !EEPROM_Writing) {
          EEPROM_FixtWrite[0] = 0;
          EEPROM_FixtWrite[1] = 0;
          while (ParseIndex < Bytes) {
            uint16_t Val = ParseInt(Buff, ParseIndex, Bytes);
            ParseIndex++;
            if (Val < 8) {
              Serial.print(Val);
              Serial.print(" ");
              EEPROM_FixtWrite[0] |= (1 << Val);
            }
            else if (Val == 8) {
              Serial.print(Val);
              Serial.print(" ");
              EEPROM_FixtWrite[1] = 1;
            }
          }
          EEPROM_Writing = true;
        }
        Serial.println();
      }
      if (!strcmp(CmdStr, "RnMc")) {
        Serial.print("SrMc ");
        uint8_t ParseIndex = 5;
        uint8_t LoadMacroVal = ParseInt(Buff, ParseIndex, Bytes);
        Serial.print(LoadMacroVal);
        Serial.print(" ");
        ParseIndex++;
        uint16_t MacroDelay = ParseInt(Buff, ParseIndex, Bytes);
        Serial.println(MacroDelay);
        if (LoadMacroVal < 20) {
          LoadMacro(LoadMacroVal);
          uint16_t StepsToFade = (MacroDelay / 33);
          DmxMacro.RunMacro(StepsToFade);
        }
      }
      if (!strcmp(CmdStr, "DeTr")) {
        Serial.print("DeTr ");
        uint8_t ParseIndex = 5;
        uint16_t MacroDelay = ParseInt(Buff, ParseIndex, Bytes);
        Serial.println(MacroDelay);
        EEPROM.write(1000, (MacroDelay / 33));
        EEPROM.write(1001, ((MacroDelay / 33) >> 8));
      }
      if (!strcmp(CmdStr, "SpMc")) {
        Serial.print("SpMc");
        DmxMacro.StopMacro();
      }
      if (!strcmp(CmdStr, "TgSt")) {
        Serial.print("TgSt");
//        for (uint8_t X = 0; X < 45; X++) DMX_TAR_BUFF[X] = DMX_BUFF[X];

        uint16_t StageLightAvg = 0;
        for (uint8_t X = 0; X < 4; X++) StageLightAvg += DMX_BUFF[(8 * 5) + X];
        StageLightAvg /= 4;

        if (StageLightAvg > 127) {
          //Toggle Off:
          for (uint8_t X = 0; X < 4; X++) DMX_BUFF[(8 * 5) + X] = 0;
        }
        else {
          //Toggle On:
          for (uint8_t X = 0; X < 4; X++) DMX_BUFF[(8 * 5) + X] = 255;
        }
//        DmxMacro.RunMacro((EEPROM.read(1000) | (EEPROM.read(1001) << 8)));
      }
      if (!strcmp(CmdStr, "TgBl")) {
        Serial.print("TgBl");
        for (uint8_t X = 0; X < 45; X++) DMX_TAR_BUFF[X] = DMX_BUFF[X];

        uint16_t StageLightAvg = 0;
        for (uint8_t X = 0; X < 8; X++) StageLightAvg += DMX_BUFF[(5 * X)];
        StageLightAvg /= 8;

        if (StageLightAvg > 127) {
          //Toggle Off:
          for (uint8_t X = 0; X < 8; X++) DMX_TAR_BUFF[(5 * X)] = 0;
        }
        else {
          //Toggle On:
          for (uint8_t X = 0; X < 8; X++) DMX_TAR_BUFF[(5 * X)] = 255;
        }
        DmxMacro.RunMacro((EEPROM.read(1000) | (EEPROM.read(1001) << 8)));
      }
    }
  }
  {
    static uint32_t DMX_Tim = millis();
    if (millis() - DMX_Tim >= 33) {
      DMX_Tim += 33;
      Dmx.Run();
      DmxMacro.StepMacro();
    }
  }
  WriteEEPROM();
}

void LoadMacro(uint8_t Macro) {
  if (Macro < 20) {
    for (uint8_t X = 0; X < 45; X++) DMX_TAR_BUFF[X] = DMX_BUFF[X];

    uint16_t FixtLoad = EEPROM.read((50 * Macro) + 45) | (EEPROM.read((50 * Macro) + 45) << 8);
    for (uint8_t Y = 0; Y < 9; Y++) {
      if ((FixtLoad & (1 << Y))) {
        for (uint8_t X = 0; X < 5; X++) {
          DMX_TAR_BUFF[(5 * Y) + X] = EEPROM.read((50 * Macro) + ((5 * Y) + X));
        }
      }
    }
  }
}

void WriteEEPROM() {
  static uint8_t EEPROM_WriteByte = 0;
  if (EEPROM_Writing) {
    if (EEPROM_WriteByte < 45) {
      EEPROM.write((50 * EEPROM_WriteMacro) + EEPROM_WriteByte, DMX_BUFF[EEPROM_WriteByte]);
      EEPROM_WriteByte++;
    }
    else if (EEPROM_WriteByte == 45) {
      EEPROM.write((50 * EEPROM_WriteMacro) + EEPROM_WriteByte, EEPROM_FixtWrite[0]);
      EEPROM_WriteByte++;
    }
    else if (EEPROM_WriteByte == 46) {
      EEPROM.write((50 * EEPROM_WriteMacro) + EEPROM_WriteByte, EEPROM_FixtWrite[1]);
      EEPROM_WriteByte++;
    }
    else if (EEPROM_WriteByte < 50) {
      EEPROM.write((50 * EEPROM_WriteMacro) + EEPROM_WriteByte, 0);
      EEPROM_WriteByte++;
    }
    else {
      EEPROM_WriteByte = 0;
      EEPROM_Writing = false;
    }
  }
  else EEPROM_WriteByte = 0;
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
    if (RunCnt) return Answer;
    else RunCnt = true;
  }
  return Answer;
}
