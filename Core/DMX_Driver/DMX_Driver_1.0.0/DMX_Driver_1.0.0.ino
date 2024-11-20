#define DMX_FIXTURE_CNT 9
#define DMX_FIXT_LIST {1, 33, 65, 97, 129, 161, 193, 225, 257}
#define DMX_FIXT_CHNL 5

#include "DMX_Driver.h"

DMX_Driver dmx(3);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  dmx.Run();
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
        dmx.Write(fixture, channel, value);
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
