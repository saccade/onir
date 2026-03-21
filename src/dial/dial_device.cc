#include "dial_device.h"

#include "Arduino.h"

DialDevice::DialDevice(const Hardware& hardware) : hardware(hardware) {
  if (not empty(hardware)) {
    pinMode(dispatch(hardware, Function::CLOCK),  INPUT_PULLUP);
    pinMode(dispatch(hardware, Function::DATA),   INPUT_PULLUP);
    pinMode(dispatch(hardware, Function::SWITCH), INPUT_PULLUP);
  }
  clock = true;   // assume the recent past was boring.
}

bool DialDevice::clock_pin() const {
  return digitalRead(dispatch(hardware, Function::CLOCK));
}

bool DialDevice::data_pin() const {
  return digitalRead(dispatch(hardware, Function::DATA));
}

bool DialDevice::switch_pressed() const {
  return not digitalRead(dispatch(hardware, Function::SWITCH));
}

void DialDevice::read(DialState& state) {
  bool old_clock = clock;
  clock = clock_pin();
  if (clock != old_clock) {
    if (clock) {  // rising edge only
      if (data_pin()) {
        if (switch_pressed()) {
          state.down_count--;
        } else {
          state.count--;
        }
      } else {
        if (switch_pressed()) {
          state.down_count++;
        } else {
          state.count++;
        }
      }
    }
  }

  state.button = switch_pressed();
}
