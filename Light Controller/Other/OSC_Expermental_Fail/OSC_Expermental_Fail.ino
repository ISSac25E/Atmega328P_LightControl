#include <OSCMessage.h>

/*
    Make an OSC message and send it over UDP
    
    Adrian Freed
 */
#include <UIPEthernet.h>
#include <UIPUdp.h>
#include <SPI.h>    
#include <OSCMessage.h>

EthernetUDP Udp;

//the Arduino's IP
IPAddress ip(192, 168, 86, 176);
//destination IP
IPAddress outIp(192, 168, 86, 138);
const unsigned int outPort = 12321;

 byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // you can find this written on the board of some Arduino Ethernets or shields
void setup() {
  Ethernet.begin(mac,ip);
    Udp.begin(8888);
OSCMessage msg("/style/bgcolor/1/2");
  msg.add(255);
  msg.add(0);
  msg.add(0);
  
  Udp.beginPacket(outIp, outPort);
    msg.send(Udp); // send the bytes to the SLIP stream
  Udp.endPacket(); // mark the end of the OSC Packet
  msg.empty(); // free space occupied by message
 
}


void loop(){
  //the message wants an OSC address as first argument


}
