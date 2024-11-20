//UDP_Parse REV 1.0.0
//.h
#ifndef UDP_Parse_h
#define UDP_Parse_h

#include "Arduino.h"

#include <Ethernet.h>
#include <EthernetUdp.h>

#define UDP_Parse_MaxBytes 39

class UDP_Parse {
  public:
    Begin(uint16_t Port) {
      _UDP.begin(Port);
    }
    bool UDP_Avail();
    uint8_t ParseCMD(char *Data);
  private:
    EthernetUDP _UDP;
};

//.cpp
//#include "UDP_DMX_Parse.h"
//#include "Arduino.h"


bool UDP_Parse::UDP_Avail() {
  static bool UDP_Reading = false;
  if (!UDP_Reading && _UDP.parsePacket()) {
    UDP_Reading = true;
  }
  else {
    if(!_UDP.available()) UDP_Reading = false; 
  }
  return UDP_Reading;
}

uint8_t UDP_Parse::ParseCMD(char *Data) {
  while (_UDP.available()) {
    if (_UDP.read() == ':') {
      uint8_t Bytes = 0;
      while(_UDP.available()) {
        char CharHold = _UDP.read();
        if (CharHold == ';') {
          return Bytes;
        }
        else {
          Data[Bytes] = CharHold;
          if(Bytes < UDP_Parse_MaxBytes) Bytes++;
        }
      }
    }
  }
  return 0;
}
#endif
