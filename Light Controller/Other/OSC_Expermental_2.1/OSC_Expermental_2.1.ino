#include <OSCMessage.h>

/*
    Make an OSC message and send it over UDP

    Adrian Freed
*/
#include <UIPEthernet.h>
#include <UIPUdp.h>
#include <SPI.h>

EthernetUDP Udp;
EthernetUDP UdpRX;

//the Arduino's IP
IPAddress ip(192, 168, 86, 177);
//destination IP
IPAddress outIp(192, 168, 86, 138);
const unsigned int outPort = 12321;

byte mac[] = {
  0xFD, 0xE0, 0xE0, 0x59, 0xC2, 0x5C
}; // you can find this written on the board of some Arduino Ethernets or shields
void setup() {
  Serial.begin(115200);
  Ethernet.begin(mac, ip);
  Udp.begin(27675);
}


void loop() {
  for (uint8_t X = 1; X <= 32; X++) {
    char C[20] = "/style/bgcolor/6/";
    if(X > 9) {
      C[17] = (X / 10) + 48;
      C[18] = (X % 10) + 48;
    }
    else {
    C[17] = (X + 48);
    }
    OSCMessage msg(C);
    msg.add(255);
    msg.add(0);
    msg.add(0);

    Udp.beginPacket(outIp, outPort);
    msg.send(Udp); // send the bytes to the SLIP stream
    Udp.endPacket(); // mark the end of the OSC Packet
    msg.empty(); // free space occupied by message
    Udp.flush();
  }
  delay(1000);
  for (uint8_t X = 1; X <= 32; X++) {
    char C[20] = "/style/bgcolor/6/";
    if(X > 9) {
      C[17] = (X / 10) + 48;
      C[18] = (X % 10) + 48;
    }
    else {
    C[17] = (X + 48);
    }
    OSCMessage msg(C);
    msg.add(0);
    msg.add(255);
    msg.add(0);

    Udp.beginPacket(outIp, outPort);
    msg.send(Udp); // send the bytes to the SLIP stream
    Udp.endPacket(); // mark the end of the OSC Packet
    msg.empty(); // free space occupied by message
    Udp.flush();
  }
  delay(1000);
  
  {
    char C[20] = "/style/bgcolor/6/";
    C[17] = (2 + 48);
    OSCMessage msg(C);
    msg.add(255);
    msg.add(0);
    msg.add(0);

    Udp.beginPacket(outIp, outPort);
    msg.send(Udp); // send the bytes to the SLIP stream
    Udp.endPacket(); // mark the end of the OSC Packet
    msg.empty(); // free space occupied by message
  }
  delay(1000);
  {
    char C[] = "/style/bgcolor/6/";
    //    str += (2 + 48);
    OSCMessage msg(C);
    msg.add(0);
    msg.add(255);
    msg.add(0);

    Udp.beginPacket(outIp, outPort);
    msg.send(Udp); // send the bytes to the SLIP stream
    Udp.endPacket(); // mark the end of the OSC Packet
    msg.empty(); // free space occupied by message
  }
  delay(1000);

}
