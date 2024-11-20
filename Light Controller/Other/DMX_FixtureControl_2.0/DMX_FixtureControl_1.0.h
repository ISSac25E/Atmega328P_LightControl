#ifndef DMX_FixtureControl_h
#define DMX_FixtureControl_h

#include "Arduino.h"

#include <DmxSimple.h>

class DMX_FixtureControl
{
  public:
    DMX_FixtureControl();
    void DMX_SetFixture(uint8_t FixtureNumber, uint8_t *NewFixtureValue);
    void FixtureWrite(uint16_t Frames);
    bool DMX_Run();
  private:
    const uint16_t _FixtureChannels[10][8] = 
   {{1, 2, 3, 4, 5, 0, 0, 0},
    {33, 34, 35, 36, 37, 0, 0, 0},
    {65, 66, 67, 68, 69, 0, 0, 0},
    {97, 98, 99, 100, 101, 0, 0, 0},
    {129, 130, 131, 132, 133, 0, 0, 0},
    {161, 162, 163, 164, 165, 0, 0, 0},
    {193, 194, 195, 196, 197, 0, 0, 0},
    {225, 226, 227, 228, 229, 0, 0, 0},
    {257, 258, 259, 260, 261, 0, 0, 0},
    {289, 290, 291, 292, 293, 0, 0, 0}};
    
    uint8_t _FixtureCurrentValue[10][8];
    
    uint8_t _FixturePrevValue[10][8];
    
    uint8_t _FixtureDestValue[10][8];
    
    uint16_t _FixtureFrameCount = 0;
    uint16_t _FixtureFrames = 0;
    uint32_t FixtureTimer = 0;
    
};

#endif


//#include "DMX_FixtureControl.h"

DMX_FixtureControl::DMX_FixtureControl() {
  DmxSimple.usePin(3);
  memset(_FixtureCurrentValue, 0, 8*10);
  memset(_FixtureDestValue, 0, 8*10);
}

void DMX_FixtureControl::DMX_SetFixture(uint8_t FixtureNumber, uint8_t *NewFixtureValue) {
  if(FixtureNumber < 10) {
  for (uint8_t X = 0; X < 8; X++) _FixtureDestValue[FixtureNumber][X] = NewFixtureValue[X];
  for (uint8_t X = 0; X < 8; X++) _FixturePrevValue[FixtureNumber][X] = _FixtureCurrentValue[FixtureNumber][X];
  }
}

void DMX_FixtureControl::FixtureWrite(uint16_t Frames) {
  _FixtureFrameCount = Frames;
  if (!_FixtureFrameCount) _FixtureFrameCount = 1;
  _FixtureFrames = _FixtureFrameCount;
}

bool DMX_FixtureControl::DMX_Run() {
  if (millis() - FixtureTimer >= 33) {
    FixtureTimer = millis();
    if (_FixtureFrameCount) {
      _FixtureFrameCount--;
      for(uint8_t Y = 0; Y < 10; Y++) {
      for (uint8_t X = 0; X < 8; X++) {
        if (_FixtureChannels[Y][X]) {
          _FixtureCurrentValue[Y][X] = map(_FixtureFrameCount, _FixtureFrames, 0, _FixturePrevValue[Y][X], _FixtureDestValue[Y][X]);
          DmxSimple.write(_FixtureChannels[Y][X], _FixtureCurrentValue[Y][X]);
//          Serial.print(_FixtureCurrentValue[Y][X]);
//          Serial.print(",");
        }
      }

//      Serial.print(":");
    }
//    Serial.println();
if(!_FixtureFrameCount) {
  for(uint8_t Y = 0; Y < 10; Y++) 
  for (uint8_t X = 0; X < 8; X++) _FixturePrevValue[Y][X] = _FixtureCurrentValue[Y][X];
}
    return true;
    }
  }
  if (_FixtureFrameCount)return true;
  return false;
}
