#include "DMX_FixtureControl_1.0.h"
DMX_FixtureControl dmx;
void setup() {
  Serial.begin(115200);
  {
  byte NewDest[] = {255,0,0,0,0};
  dmx.FixtureWrite(NewDest, 1);
  while(dmx.DMX_Run());
  }

}

void loop() {
  {
  byte NewDest[] = {255,255,0,0,0};
  dmx.FixtureWrite(NewDest, 30);
  while(dmx.DMX_Run());
  }
  delay(50);
  {
  byte NewDest[] = {255,0,255,0,0};
  dmx.FixtureWrite(NewDest, 30);
  while(dmx.DMX_Run());
  }
  delay(50);
  {
  byte NewDest[] = {255,0,0,255,0};
  dmx.FixtureWrite(NewDest, 30);
  while(dmx.DMX_Run());
  }
  delay(50);
}
