#include "hardware.h"

const Hardware no_hardware = { };
const int hardware_size = (int)Function::COUNT;

const int min_channel = 0x08;        // = 8 -- https://i2cdevices.org/addresses
const int max_channel = 0x77;        // 0x77 = 119

int assign(Hardware hardware, Function fn, int pin) {
  return hardware[(int)fn] = pin;
}

void clear(Hardware hardware) {
  for (int fn = (int)Function::NONE + 1; fn < (int)Function::COUNT - 1; fn++) {
    assign(hardware, (Function)fn, UNSET); // it's rude to yell at the modem
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
