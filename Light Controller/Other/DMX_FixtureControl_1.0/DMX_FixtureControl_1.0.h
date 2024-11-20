#ifndef DMX_FixtureControl_h
#define DMX_FixtureControl_h

#include "Arduino.h"

#include <DmxSimple.h>

class DMX_FixtureControl
{
  public:
    DMX_FixtureControl();
    void FixtureWrite(uint8_t *NewFixtureValue, uint16_t Frames);
    bool DMX_Run();
  private:
    const uint16_t _FixtureChannelCount = 5;
    const uint16_t _FixtureChannels[8] = {33, 34, 35, 36, 37, 0, 0, 0};
    uint8_t _FixtureCurrentValue[5];
    uint8_t _FixturePrevValue[5];
    uint8_t _FixtureDestValue[5];
    uint16_t _FixtureFrameCount = 0;
    uint16_t _FixtureFrames = 0;
    uint32_t FixtureTimer = 0;
    
};

#endif


//#include "DMX_FixtureControl.h"

DMX_FixtureControl::DMX_FixtureControl() {
  DmxSimple.usePin(3);
  memset(_FixtureCurrentValue, 0, _FixtureChannelCount);
}

void DMX_FixtureControl::FixtureWrite(uint8_t *NewFixtureValue, uint16_t Frames) {
  _FixtureFrameCount = Frames;
  if (!_FixtureFrameCount) _FixtureFrameCount = 1;
  _FixtureFrames = _FixtureFrameCount;
  for (uint8_t X = 0; X < _FixtureChannelCount; X++) _FixtureDestValue[X] = NewFixtureValue[X];
  for (uint8_t X = 0; X < _FixtureChannelCount; X++) _FixturePrevValue[X] = _FixtureCurrentValue[X];
}

bool DMX_FixtureControl::DMX_Run() {
  if (millis() - FixtureTimer >= 33) {
    FixtureTimer = millis();
    if (_FixtureFrameCount) {
      _FixtureFrameCount--;
      for (uint8_t X = 0; X < _FixtureChannelCount; X++) {
        if (_FixtureChannels[X]) {
          _FixtureCurrentValue[X] = map(_FixtureFrameCount, _FixtureFrames, 0, _FixturePrevValue[X], _FixtureDestValue[X]);
          DmxSimple.write(_FixtureChannels[X], _FixtureCurrentValue[X]);
          Serial.print(_FixtureCurrentValue[X]);
          Serial.print(",");
        }
      }
      Serial.println();
      return true;
    }
  }
  if (_FixtureFrameCount)return true;
  return false;
}
