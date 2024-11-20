#include "DMX_FixtureControl_1.0.h"
DMX_FixtureControl dmx;
void setup() {
  Serial.begin(115200);

}

void loop() {
  
  {
  byte NewDest[] = {255,255,0,0,0,0,0,0};
dmx.DMX_SetFixture(1, NewDest);
  }
  dmx.FixtureWrite(5);
  while(dmx.DMX_Run());

  {
  byte NewDest[] = {255,0,0,0,0,0,0,0};
dmx.DMX_SetFixture(1, NewDest);
  }
  {
  byte NewDest[] = {255,255,0,0,0,0,0,0};
dmx.DMX_SetFixture(2, NewDest);
  }
  dmx.FixtureWrite(5);
  while(dmx.DMX_Run());

  {
  byte NewDest[] = {255,0,0,0,0,0,0,0};
dmx.DMX_SetFixture(2, NewDest);
  }
  {
  byte NewDest[] = {255,255,0,0,0,0,0,0};
dmx.DMX_SetFixture(3, NewDest);
  }
  dmx.FixtureWrite(5);
  while(dmx.DMX_Run());

  {
  byte NewDest[] = {255,0,0,0,0,0,0,0};
dmx.DMX_SetFixture(3, NewDest);
  }
  {
  byte NewDest[] = {255,255,0,0,0,0,0,0};
dmx.DMX_SetFixture(4, NewDest);
  }
  dmx.FixtureWrite(5);
  while(dmx.DMX_Run());

    {
  byte NewDest[] = {255,0,0,0,0,0,0,0};
dmx.DMX_SetFixture(4, NewDest);
  }
  {
  byte NewDest[] = {255,255,0,0,0,0,0,0};
dmx.DMX_SetFixture(6, NewDest);
  }
  dmx.FixtureWrite(5);
  while(dmx.DMX_Run());

  {
  byte NewDest[] = {255,0,0,0,0,0,0,0};
dmx.DMX_SetFixture(6, NewDest);
  }
  {
  byte NewDest[] = {255,255,0,0,0,0,0,0};
dmx.DMX_SetFixture(7, NewDest);
  }
  dmx.FixtureWrite(5);
  while(dmx.DMX_Run());
  {
  byte NewDest[] = {255,0,0,0,0,0,0,0};
dmx.DMX_SetFixture(7, NewDest);
  }
  dmx.FixtureWrite(5);
  while(dmx.DMX_Run());
delay(300);
  
//  {
//  byte NewDest[] = {255,255,0,0,0,0,0,0};
////  dmx.DMX_SetFixture(0, NewDest);
//  dmx.DMX_SetFixture(1, NewDest);
//  dmx.DMX_SetFixture(2, NewDest);
//  dmx.DMX_SetFixture(3, NewDest);
//  dmx.DMX_SetFixture(4, NewDest);
////  dmx.DMX_SetFixture(5, NewDest);
//  dmx.DMX_SetFixture(6, NewDest);
//  dmx.DMX_SetFixture(7, NewDest);
//  dmx.FixtureWrite(300);
//  while(dmx.DMX_Run());
//  }
//  delay(400);
//  {
//  byte NewDest[] = {255,0,255,0,0,0,0,0};
////  dmx.DMX_SetFixture(0, NewDest);
//  dmx.DMX_SetFixture(1, NewDest);
//  dmx.DMX_SetFixture(2, NewDest);
//  dmx.DMX_SetFixture(3, NewDest);
//  dmx.DMX_SetFixture(4, NewDest);
////  dmx.DMX_SetFixture(5, NewDest);
//  dmx.DMX_SetFixture(6, NewDest);
//  dmx.DMX_SetFixture(7, NewDest);
//  dmx.FixtureWrite(300);
//  while(dmx.DMX_Run());
//  }
//  delay(400);
//  {
//  byte NewDest[] = {255,0,0,255,0,0,0,0};
////  dmx.DMX_SetFixture(0, NewDest);
//  dmx.DMX_SetFixture(1, NewDest);
//  dmx.DMX_SetFixture(2, NewDest);
//  dmx.DMX_SetFixture(3, NewDest);
//  dmx.DMX_SetFixture(4, NewDest);
////  dmx.DMX_SetFixture(5, NewDest);
//  dmx.DMX_SetFixture(6, NewDest);
//  dmx.DMX_SetFixture(7, NewDest);
//  dmx.FixtureWrite(300);
//  while(dmx.DMX_Run());
//  }
//  delay(400);
}
