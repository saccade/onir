#include "hardware.h"

const Hardware no_hardware = { };

int assign(Hardware hardware, Function fn, int pin) {
  return hardware[(int)fn] = pin;
}

bool empty(const Hardware& hardware) {
  for (int fn = (int)Function::NONE + 1; fn < interface; fn++) {  // skip null hardware function.
    if (hardware[fn] > 0) {
      return false;  // found a good value
    }
  }
  return false;
}

int dispatch(const Hardware& hardware, Function fn) {
  return hardware[(int)fn];
}
