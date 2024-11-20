#define VarMacro_BYTES 4

#include "VarMacro.h"

uint8_t Curr[] = {20, 2, 4, 2};
uint8_t Tar[] = {0, 23, 4, 255};

VarMacro Var(Curr, Tar);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("INIT");
  Var.RunMacro(30);
  for (uint8_t X = 0; X < 40; X++) {
    Var.StepMacro();
    Serial.print(X);
    Serial.print(": ");
    Serial.print(Curr[0]);
    Serial.print(",");
    Serial.print(Curr[1]);
    Serial.print(",");
    Serial.print(Curr[2]);
    Serial.print(",");
    Serial.println(Curr[3]);
  }
  Tar[0] = 255;
  Tar[1] = 1;
  Tar[2] = 5;
  Tar[3] = 0;
  Var.RunMacro(10000);
  Serial.println();
  Serial.println();
  for (uint16_t X = 0; X < 10005; X++) {
    Var.StepMacro();
    Serial.print(X);
    Serial.print(": ");
    Serial.print(Curr[0]);
    Serial.print(",");
    Serial.print(Curr[1]);
    Serial.print(",");
    Serial.print(Curr[2]);
    Serial.print(",");
    Serial.println(Curr[3]);
  }

  Tar[0] = 23;
  Tar[1] = 123;
  Tar[2] = 100;
  Tar[3] = 5;

  Var.RunMacro(30);
  Serial.println();
  Serial.println();
  for (uint8_t X = 0; X < 40; X++) {
    if (X == 20) Var.StopMacro();
    Var.StepMacro();
    Serial.print(X);
    Serial.print(": ");
    Serial.print(Curr[0]);
    Serial.print(",");
    Serial.print(Curr[1]);
    Serial.print(",");
    Serial.print(Curr[2]);
    Serial.print(",");
    Serial.println(Curr[3]);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
