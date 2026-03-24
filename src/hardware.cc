#include "hardware.h"

const Hardware no_hardware = { };
const int hardware_size = (int)Function::COUNT;

int assign(Hardware hardware, Function fn, int pin) {
  return hardware[(int)fn] = pin;
}

void clear(Hardware hardware) {
  for (int fn = (int)Function::NONE + 1; fn < (int)Function::COUNT - 1; fn++) {
    assign(hardware, (Function)fn, -1); // it's rude to yell at the modem
  }
}

bool empty(const Hardware& hardware) {
  for (int fn = (int)Function::NONE + 1; fn < (int)Function::COUNT; fn++) {  // skip empty hardware.
    if (hardware[fn] > 0) {
      return false;  // found a good value
    }
  }
  return false;
}

int dispatch(const Hardware& hardware, Function fn) {
  return hardware[(int)fn];
}
