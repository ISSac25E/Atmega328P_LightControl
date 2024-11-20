#include <SPI.h>
#include <UIPEthernet.h>

EthernetUDP COMP_TX;

byte ARD_MAC[] = {0x48, 0xF9, 0x72, 0xAC, 0x5C, 0x5E};
IPAddress ARD_IP(192, 168, 86, 188);
IPAddress COMP_IP(192, 168, 86, 138);

UIPClient UIPClient;

long int LCP = 8888;

void setup()
{
    Ethernet.begin(ARD_MAC, ARD_IP);

    COMP_TX.begin(8282);
    Serial.begin(115200);
    Serial.println("INIT");
    while (!UIPClient.connect(COMP_IP, LCP))
    {
        delay(100);
    }
    Serial.println("Connected");
}

void loop()
{
    // if (Serial.available())
    // {
    //     delay(5);
    //     uint8_t Length = 0;
    //     char C[100];
    //     while (Serial.available() && Serial.peek() != 10 && Length < 100)
    //     {
    //         C[Length] = Serial.read();
    //         Length++;
    //     }
    //     Serial.print("STR: \"");
    //     for (uint8_t X = 0; X < Length; X++)
    //     {
    //         Serial.write(C[X]);
    //     }
    //     Serial.println('\"');
    //     Serial.print("CHAR: ");
    //     for (uint8_t X = 0; X < Length; X++)
    //     {
    //         Serial.print('[');
    //         Serial.print((uint8_t)C[X]);
    //         Serial.print(']');
    //         if (X < (Length - 1))
    //             Serial.print(",");
    //     }
    //     Serial.println();
    //     while (Serial.available())
    //         Serial.read();
    //     COMP_TX.beginPacket(COMP_IP, 50166); //WebSockeet Port
    //     COMP_TX.write(C, Length);
    //     COMP_TX.endPacket();
    //     delay(30);
    //     COMP_TX.beginPacket(COMP_IP, 51237); //UDP Port
    //     COMP_TX.write(C, Length);
    //     COMP_TX.endPacket();
    //     // delay(30);
    //     // COMP_TX.beginPacket(ARD_IP, 50166);
    //     // COMP_TX.write(C, Length);
    //     // COMP_TX.endPacket();
    //     Serial.println("MSG SENT");
    //     Serial.println();
    // }
    // if (COMP_TX.parsePacket())
    // {
    //     Serial.print("UDP MSG: ");
    //     Serial.println(COMP_TX.available());
    //     uint16_t Length = COMP_TX.available();
    //     char C[Length];
    //     COMP_TX.read(C, Length);
    //     Serial.print("STR: \"");
    //     for (uint16_t X = 0; X < Length; X++)
    //     {
    //         Serial.write(C[X]);
    //     }
    //     Serial.println('\"');
    //     Serial.print("CHAR: ");
    //     for (uint16_t X = 0; X < Length; X++)
    //     {
    //         Serial.print('[');
    //         Serial.print((uint8_t)C[X]);
    //         Serial.print(']');
    //         if (X < (Length - 1))
    //             Serial.print(",");
    //     }
    //     Serial.println();
    // }

    const byte C[] = {0x81, 0x33, 0x34, 0x32, 0x5B, 0x22, 0x76, 0x61, 0x72, 0x69, 0x61, 0x62, 0x6C, 0x65, 0x73, 0x5F, 0x73, 0x65, 0x74, 0x22, 0x2C, 0x7B, 0x22, 0x69, 0x6E, 0x74, 0x65, 0x72, 0x6E, 0x61, 0x6C, 0x3A, 0x63, 0x75, 0x73, 0x74, 0x6F, 0x6D, 0x5F, 0x54, 0x65, 0x73, 0x74, 0x22, 0x3A, 0x22, 0x38, 0x22, 0x7D, 0x2C, 0x5B, 0x5D, 0x5D};
    for (uint16_t X = 0; X < sizeof(C); X++)
        UIPClient.write(C[X]);
    delay(100);
}