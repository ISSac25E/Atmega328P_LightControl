//#ifdef SerialEnable
//#endif

#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

#define SerialEnable

byte ARD_MAC[] = {0x92, 0x86, 0xB5, 0xDD, 0x39, 0x1B};
IPAddress ARD_IP(192, 168, 86, 22);
EthernetServer ARD_Server(80);
File WebFile;

void setup() {
#ifdef SerialEnable
  Serial.begin(115200);
  Serial.println("Arduino SD WebServer");
  Serial.println("Initializing SD card...");
#endif
  while (!SD.begin(4)) {
#ifdef SerialEnable
    Serial.println("ERROR - SD Card Initialization Failed");
    Serial.print("Trying again in 5 Seconds: ");
#endif
    delay(5000);
  }
#ifdef SerialEnable
  Serial.println("SD Card Initialized");
#endif
  if (!SD.exists("INDEX.HTM")) {
#ifdef SerialEnable
    Serial.println("ERROR - Can't Find INDEX.HTML File");
#endif
    while (1);
  }
#ifdef SerialEnable
  Serial.println("INDEX.HTML File Located");
#endif
  Ethernet.begin(ARD_MAC, ARD_IP);
#ifdef SerialEnable
  Serial.print("Arduino WebServer Located At: ");
  Serial.println(ARD_IP);
#endif
  ARD_Server.begin();


}

void loop() {
  EthernetClient CLIENT = ARD_Server.available();
  if (CLIENT) {
#ifdef SerialEnable
    Serial.print("New Client Found: ");
    Serial.println(CLIENT.localPort());
    Serial.println();
#endif
    bool LineReturn = true;
    const uint8_t HTML_RequestBufferSize = 80;
    byte HTML_RequestBuffer[HTML_RequestBufferSize];
    uint8_t HTML_RequestIndex = 0;
    for (uint8_t X = 0; X < HTML_RequestBufferSize; X++) HTML_RequestBuffer[X] = 0;
    while (CLIENT.connected()) {
      while (CLIENT.available()) {
        char C = CLIENT.read();
#ifdef SerialEnable
        Serial.write(C);
#endif
        if (HTML_RequestIndex < (HTML_RequestBufferSize - 1)) {
          HTML_RequestBuffer[HTML_RequestIndex] = C;
          HTML_RequestIndex++;
        }
        if (C == '\n' and LineReturn) {
#ifdef SerialEnable
          Serial.println();
#endif
          uint8_t BufferFoundStringIndex = StringContains(HTML_RequestBuffer, "GET /");
          if (BufferFoundStringIndex) {
            BufferFoundStringIndex++;
            String RequestedFile = "";
            while (HTML_RequestBuffer[BufferFoundStringIndex] != ' ') {
              RequestedFile += char(HTML_RequestBuffer[BufferFoundStringIndex]);
              BufferFoundStringIndex++;
            }
            if (RequestedFile == "") {
              WebFile = SD.open("INDEX.HTM");
//              if (WebFile) {
//                CLIENT.println("HTTP/1.1 200 OK");
//                CLIENT.println("Content-Type: text/html");
//                CLIENT.println("Connnection: close");
//                CLIENT.println();
//              }
            }
            else {
              WebFile = SD.open(RequestedFile);
//              if (WebFile) {
//                CLIENT.println("HTTP/1.1 200 OK");
//                CLIENT.println();
//              }
            }
            if (WebFile) {
#ifdef SerialEnable
              Serial.print("Sending File: ");
              Serial.println(RequestedFile);
#endif
              byte SD_Buffer[100];
              byte SD_BufferCounter = 0;
              while (WebFile.available()) {
                SD_Buffer[SD_BufferCounter] = WebFile.read();
                SD_BufferCounter++;
                if (SD_BufferCounter == 100) {
                  CLIENT.write(SD_Buffer, 100); // send web page to client
                  SD_BufferCounter = 0;
                }
              }
              if (SD_BufferCounter) CLIENT.write(SD_Buffer, SD_BufferCounter);
              WebFile.close();
            }
            else {
#ifdef SerialEnable
              Serial.print("ERROR - File Not Found: ");
              Serial.println(RequestedFile);
#endif
            }
          }
          else {
#ifdef SerialEnable
            Serial.println("ERROR - Invalide Message");
#endif
          }
        }
        else if (C == '\n') LineReturn = true;
        else if (C != '\r') LineReturn = false;
      }
    delay(1);
    CLIENT.stop();
    CLIENT.flush();
    }
  }
}

void StringClear(char *Str, char Length) {
  for (uint16_t X = 0; X < Length; X++) Str[X] = 0;
}

uint16_t StringContains(char *Str, char *FindStr) {
  uint8_t CharFound = 0;
  uint16_t CharIndex = 0;
  uint8_t Length = strlen(Str);

  if (strlen(FindStr) > Length) return 0;
  while (CharIndex < Length) {
    if (Str[CharIndex] == FindStr[CharFound]) {
      CharFound++;
      if (strlen(FindStr) == CharFound) return CharIndex;
    }
    else CharFound = 0;
    CharIndex++;
  }
  return 0;
}
