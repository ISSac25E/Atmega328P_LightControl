#define DMX_FIXTURE_CNT 9
#define DMX_FIXT_LIST {1, 33, 65, 97, 129, 161, 193, 225, 257}
#define DMX_FIXT_CHNL 5

#include "DMX_Driver.h"

uint8_t DMX_Buff[DMX_FIXTURE_CNT * DMX_FIXT_CHNL];

DMX_Driver dmx(3, DMX_Buff);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(100);
}

void loop() {
  static uint32_t DMX_Tim = millis();
  if (millis() - DMX_Tim >= 33) {
    DMX_Tim += 33;
    dmx.Run();
  }
  while (Serial.available()) {
    char c = Serial.read();
    static int value = 0;
    static int fixture;
    static int channel;
    if ((c >= '0') && (c <= '9')) {
      value = 10 * value + c - '0';
    } else {
      if (c == 'f') fixture = value;
      else if (c == 'c') {
        channel = value;
      }
      else if (c == 'v') {
        DMX_Buff[dmx.ChnlAdd(fixture, channel)] = value;
        Serial.print(fixture);
        Serial.print(' ');
        Serial.print(channel);
        Serial.print(' ');
        Serial.println(value);
      }
      value = 0;
    }
  }
}
