// H

#ifndef OptiDMX_h
#define OptiDMX_h

#include "Arduino.h"

class DmxOptiClass
{
  public:
    uint16_t FixtureChannels[10];
    volatile void Begin();
    volatile void Begin(uint8_t Pin);
    volatile void End();
    void SetPin(uint8_t Pin);
    void Write(uint8_t *Value);
    void Write(uint8_t Fixture, uint8_t *Value);
    void Write(uint8_t Fixture, uint8_t Channel, uint8_t Value);
    void SetFixture(uint8_t FixtureNum, uint16_t FixtureAdd);
    void SetFixture(uint16_t *FixtureAdd);
  private:
    volatile void ISR_SendByte();

    volatile uint8_t DmxBuffer[10][8];
    volatile bool DmxInit = false;
    volatile uint8_t DmxPin = 3;
    volatile uint8_t *PinPort;
    volatile uint8_t PinMask;
};
extern DmxOptiClass DmxOpti;

#endif

// CPP

//#include "OptiDMX.h"

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB1286__)
#define TIMER2_INTERRUPT_ENABLE() TIMSK2 |= _BV(TOIE2)
#define TIMER2_INTERRUPT_DISABLE() TIMSK2 &= ~_BV(TOIE2)
#define ISR_NAME TIMER2_OVF_vect
#define BITS_PER_TIMER_TICK (F_CPU / 31372)
#else
#error "CPU Not Supported"
#endif

void DmxOptiClass::SetPin(uint8_t Pin) {
  if (DmxInit) End();
  DmxPin = Pin;
}
void DmxOptiClass::Begin() {
  if (DmxPin >= 0 && DmxPin <= 13) {
    if (DmxPin >= 0 and DmxPin <= 7) {
      PinPort = &PORTD;
      uint8_t TempPinMask = 1;
      TempPinMask << DmxPin;
      PinMask = TempPinMask;
    }
    else if (DmxPin >= 8 and DmxPin <= 13) {
      PinPort = &PORTB;
      uint8_t TempPinMask = 1;
      TempPinMask << DmxPin - 8;
      PinMask = TempPinMask;
    }
    if (DmxInit) End();
    DmxInit = true;
    pinMode(DmxPin, OUTPUT);
    TIMER2_INTERRUPT_ENABLE();
  }
}

void DmxOptiClass::Begin(uint8_t Pin) {
  DmxPin = Pin;
  if (DmxPin >= 0 && DmxPin <= 13) {
    if (DmxPin >= 0 and DmxPin <= 7) {
      PinPort = &PORTD;
      uint8_t TempPinMask = 1;
      TempPinMask << DmxPin;
      PinMask = TempPinMask;
    }
    else if (DmxPin >= 8 and DmxPin <= 13) {
      PinPort = &PORTB;
      uint8_t TempPinMask = 1;
      TempPinMask << DmxPin - 8;
      PinMask = TempPinMask;
    }
    if (DmxInit) End();
    DmxInit = true;
    pinMode(DmxPin, OUTPUT);
    TIMER2_INTERRUPT_ENABLE();
  }
}
void DmxOptiClass::End() {
  pinMode(DmxPin, INPUT);
  TIMER2_INTERRUPT_DISABLE();
  DmxInit = false;
  memset(DmxBuffer, 0, 10 * 8);
}

void DmxOptiClass::Write(uint8_t *Value) {

}
void DmxOptiClass::Write(uint8_t Fixture, uint8_t *Value) {

}
void DmxOptiClass::Write(uint8_t Fixture, uint8_t Channel, uint8_t Value) {

}





void DmxOptiClass::ISR_SendByte(uint8_t value) {
  uint32_t begin, target;
  uint8_t mask;

  noInterrupts();
  ARM_DEMCR |= ARM_DEMCR_TRCENA;
  ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
  begin = ARM_DWT_CYCCNT;
  *dmxPort = 0;
  target = F_CPU / 250000;
  while (ARM_DWT_CYCCNT - begin < target) ;
  for (mask = 1; mask; mask <<= 1) {
    *dmxPort = (value & mask) ? 1 : 0;
    target += (F_CPU / 250000);
    while (ARM_DWT_CYCCNT - begin < target) ;
  }
  *dmxPort = 1;
  target += (F_CPU / 125000);
  while (ARM_DWT_CYCCNT - begin < target) ;
  interrupts();
}

//TODO:
//ISR(ISR_NAME,ISR_NOBLOCK) {
//
//  // Prevent this interrupt running recursively
//  TIMER2_INTERRUPT_DISABLE();
//
//  uint16_t bitsLeft = BITS_PER_TIMER_TICK; // DMX Bit periods per timer tick
//  bitsLeft >>=2; // 25% CPU usage
//  while (1) {
//    if (dmxState == 0) {
//      // Next thing to send is reset pulse and start code
//      // which takes 35 bit periods
//      uint8_t i;
//      if (bitsLeft < 35) break;
//      bitsLeft-=35;
//      *dmxPort &= ~dmxBit;
//      for (i=0; i<11; i++) delayMicroseconds(8);
//      *dmxPort |= dmxBit;
//      delayMicroseconds(12);
//      dmxSendByte(0);
//    } else {
//      // Now send a channel which takes 11 bit periods
//      if (bitsLeft < 11) break;
//      bitsLeft-=11;
//      dmxSendByte(dmxBuffer[dmxState-1]);
//    }
//    // Successfully completed that stage - move state machine forward
//    dmxState++;
//    if (dmxState > dmxMax) {
//      dmxState = 0; // Send next frame
//      break;
//    }
//  }
//
//  // Enable interrupts for the next transmission chunk
//  TIMER2_INTERRUPT_ENABLE();
//}
