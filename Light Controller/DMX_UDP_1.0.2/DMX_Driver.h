//DMX_Driver REV 1.0.1
//.h
#ifndef DMX_Driver_h
#define DMX_Driver_h

#include "Arduino.h"

#ifndef DMX_FIXTURE_CNT
#error "#DEFINE DMX_FIXTURE_CNT"
#endif

#ifndef DMX_FIXT_LIST
#error "#DEFINE DMX_FIXT_LIST"
#endif

#ifndef DMX_FIXT_CHNL
#error "#DEFINE DMX_FIXT_CHNL"
#endif

class DMX_Driver {
  public:
    DMX_Driver(uint8_t Pin, uint8_t *BufferPointer);
    void Run();
    uint16_t ChnlAdd(uint8_t Fixt, uint8_t Chnl) {
      return ((Fixt * DMX_FIXT_CHNL) + Chnl);
    }
    
  private:

    //DMX Channels to be transmitted:
    uint16_t _DMX_ChnlCnt;
    //List of all DMX Fixture Start Channels:
    const uint16_t _DMX_FixtChnl[DMX_FIXTURE_CNT] = DMX_FIXT_LIST;
    //DMX_Fixture Least to Greatest:
    uint8_t _DMX_FixtOrdrdIndex[DMX_FIXTURE_CNT];
    //Buffer Pointer, From Outside of Class:
    volatile uint8_t *_BufferPointer;

    //Vars for Pin Port Manipulation:
    volatile uint8_t *_PinPort_PIN;
    volatile uint8_t *_PinPort_DDR;
    volatile uint8_t *_PinPort_PORT;
    volatile uint8_t _BitMask;
    volatile uint8_t _BitMaskNot;

    volatile void _Send_DMX_Byte(volatile uint8_t Byte);
};

//.cpp
//#include "DMX_Control.h"
//#include "Arduino.h"

DMX_Driver::DMX_Driver(uint8_t Pin, uint8_t *BufferPointer) {
  Pin %= 20;
  if (Pin <= 7) {
    _PinPort_PIN = &PIND;
    _PinPort_DDR = (_PinPort_PIN + 1);
    _PinPort_PORT = (_PinPort_PIN + 2);
    _BitMask = (1 << (Pin));
    _BitMaskNot = ~_BitMask;
    //Port D
  }
  else if (Pin <= 13) {
    _PinPort_PIN = &PINB;
    _PinPort_DDR = (_PinPort_PIN + 1);
    _PinPort_PORT = (_PinPort_PIN + 2);
    _BitMask = (1 << (Pin - 8));
    _BitMaskNot = ~_BitMask;
    //Port B
  }
  else {
    _PinPort_PIN = &PINC;
    _PinPort_DDR = (_PinPort_PIN + 1);
    _PinPort_PORT = (_PinPort_PIN + 2);
    _BitMask = (1 << (Pin - 14));
    _BitMaskNot = ~_BitMask;
    //Port C
  }
  //Set Pin to OutPut:
  *_PinPort_PORT |= _BitMask;
  *_PinPort_DDR |= _BitMask;

  //Set Buffer Pointer:
  _BufferPointer = BufferPointer;

  {
    uint16_t LargestChannel = 0;
    for (uint8_t X = 0; X < DMX_FIXTURE_CNT; X++) {
      if (_DMX_FixtChnl[X] > LargestChannel) LargestChannel = _DMX_FixtChnl[X];
    }
    if (LargestChannel > 0)
      LargestChannel += (DMX_FIXT_CHNL - 1);
    //Make Sure it does not exceed DMX Max Channel:
    if (LargestChannel > 512) LargestChannel = 512;
    _DMX_ChnlCnt = LargestChannel;
  }

  //Generate Ordered Channels:
  for (uint8_t X = 0; X < DMX_FIXTURE_CNT; X++) _DMX_FixtOrdrdIndex[X] = 0;

  {
    uint16_t MinChnl = 1;
    for (uint8_t Y = 0; Y < DMX_FIXTURE_CNT; Y++) {
      uint8_t NextChnlIndex = 0;
      uint16_t NextChnl = _DMX_ChnlCnt;
      for (uint8_t X = 0; X < DMX_FIXTURE_CNT; X++) {
        if (_DMX_FixtChnl[X] < NextChnl && _DMX_FixtChnl[X] >= MinChnl) {
          NextChnl = _DMX_FixtChnl[X];
          NextChnlIndex = X;
        }
      }
      _DMX_FixtOrdrdIndex[Y] = NextChnlIndex;
      MinChnl = NextChnl + 1;
    }
  }
}

void DMX_Driver::Run() {
  //Timing will be done Outside. EveryTime Run is called, A Frame will be sent

  *_PinPort_PORT &= _BitMaskNot;
  delayMicroseconds(100);  //Break
  *_PinPort_PORT |= _BitMask;
  delayMicroseconds(12);  //Mark After Break
  _Send_DMX_Byte(0);  //Slot 0

  uint16_t ChannelSend = 1;
  uint8_t FixtureSend = 0;
  while (ChannelSend <= _DMX_ChnlCnt) {
    if (ChannelSend >= _DMX_FixtChnl[_DMX_FixtOrdrdIndex[FixtureSend]]) {
      for (uint8_t X = (ChannelSend - _DMX_FixtChnl[_DMX_FixtOrdrdIndex[FixtureSend]]); X < DMX_FIXT_CHNL; X++) {
        _Send_DMX_Byte(_BufferPointer[(_DMX_FixtOrdrdIndex[FixtureSend] * DMX_FIXT_CHNL) + X]);
        ChannelSend++;
      }
      FixtureSend++;
    }
    else {
      _Send_DMX_Byte(0);
      ChannelSend++;
    }
  }
}

volatile void DMX_Driver::_Send_DMX_Byte(volatile uint8_t Byte) {
  uint8_t BitCnt, DelCnt;
  asm volatile (
    "cli\n"
    "ld __tmp_reg__,%a[Port]\n"
    "and __tmp_reg__,%[MaskN]\n"
    "st %a[Port],__tmp_reg__\n"
    "ldi %[BitCnt],11\n" // 11 bit intervals per transmitted byte
    "rjmp bitLoop%=\n"     // Delay 2 clock cycles.
    "bitLoop%=:\n"\
    "ldi %[DelCnt],%[DelCntVal]\n"
    "delLoop%=:\n"
    "nop\n"
    "dec %[DelCnt]\n"
    "brne delLoop%=\n"
    "ld __tmp_reg__,%a[Port]\n"
    "and __tmp_reg__,%[MaskN]\n"
    "sec\n"
    "ror %[Byte]\n"
    "brcc sendzero%=\n"
    "or __tmp_reg__,%[Mask]\n"
    "sendzero%=:\n"
    "st %a[Port],__tmp_reg__\n"
    "dec %[BitCnt]\n"
    "brne bitLoop%=\n"
    "sei\n"
    :
    [BitCnt] "=&d" (BitCnt),
    [DelCnt] "=&d" (DelCnt)
    :
    [Port] "e" (_PinPort_PORT),
    [MaskN] "r" (_BitMaskNot),
    [Mask] "r" (_BitMask),
    [DelCntVal] "M" (13),
    [Byte] "r" (Byte)
  );
}

#endif
