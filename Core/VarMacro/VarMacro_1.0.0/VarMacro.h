//VarMacro REV 1.0.0
//.h
#ifndef VarMacro_h
#define VarMacro_h

#ifndef VarMacro_BYTES
#error "#DEFINE VarMacro_BYTES"
#endif

#include "Arduino.h"

class VarMacro {
  public:
    VarMacro(uint8_t *CurrPointer, uint8_t *TargetPointer) {
      _CurrPointer = CurrPointer;
      _TargetPointer = TargetPointer;
    }
    void RunMacro(uint16_t Steps);
    void StopMacro();
    void StepMacro();
  private:
    uint8_t _Diff(uint8_t X, uint8_t Y);
    uint8_t *_CurrPointer;
    uint8_t *_TargetPointer;
    uint8_t _StartVals[VarMacro_BYTES];

    uint16_t _StpsToRun;
    uint16_t _CurrStep;

    bool _RunMacro = false;
};

//.cpp
//#include "VarMacro.h"
//#include "Arduino.h"

void VarMacro::RunMacro(uint16_t Steps) {
  if (Steps) {
    _StpsToRun = Steps;
    _CurrStep = 0;
    for (uint16_t X = 0; X < VarMacro_BYTES; X++) _StartVals[X] = _CurrPointer[X];
    _RunMacro = true;
  }
  else {
    for (uint16_t X = 0; X < VarMacro_BYTES; X++) _CurrPointer[X] = _TargetPointer[X];
    _RunMacro = false;
  }
}

void VarMacro::StopMacro() {
  _RunMacro = false;
}

void VarMacro::StepMacro() {
  if (_RunMacro) {
    if (_StpsToRun && _CurrStep <= _StpsToRun) {
      for (uint16_t X = 0; X < VarMacro_BYTES; X++) {
        uint8_t Offset = ((uint32_t)((uint32_t)_Diff(_TargetPointer[X], _StartVals[X]) * (uint32_t)_CurrStep) / _StpsToRun);
        if(_StartVals[X] > _TargetPointer[X]) {
          _CurrPointer[X] = _StartVals[X] - Offset;
        }
        else {
          _CurrPointer[X] = _StartVals[X] + Offset;
        }
      }
      _CurrStep++;
    }
    else _RunMacro = false;
  }
}

uint8_t VarMacro::_Diff(uint8_t X, uint8_t Y) {
  if(X > Y) return X - Y;
  else return Y - X;
}

#endif
