#pragma once

#include "display_device.h"
#include "dial/dial_device.h"
#include "selector.h"

// I2C-ready device that runs a display and dial.
//
// I2C is configured at the sketch level.
// This class knows nothing about Wire, addresses, or handlers.
//
// device:
//   - state.display is written by transport layer (DisplayState)
//   - state.dial   is read    by transport layer (DialState)
//
// loop():
//   - refresh display from state.display
//   - update state.dial from hardware

class IODevice {
public:
  IODevice(const Hardware& hardware = no_hardware);

  bool new_channel = false;

  void update() {
    dial->read(buffer.dial);            // update dial buffer from hardware
    display->state = buffer.display;    // push display buffer to hardware
    display->refresh();                 // change the lights if it's time

    if (buffer.dial.button) {
      new_channel = true;
    }
  }

  IOState buffer;

private:

  DialDevice* dial;
  DisplayDevice* display;

  const Hardware& hardware;
};
