#pragma once

#include "onir.h"
#include "display_device.h"
#include "dial_device.h"
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

  int reboot_channel = -1;

  void update() {
    // Update dial state from hardware
    dial->read(state.dial);

    // Push display state to hardware (scan / refresh)
    display->state = state.display;
    display->refresh();

    if (state.dial.button) {
     Selector selector(hardware);
     selector.set_button(state.dial.button);
     reboot_channel = selector.get_channel();
    }
  }

  IOState state;   // shared storage (transport reads/writes this)

private:
  DialDevice* dial;
  DisplayDevice* display;

  const Hardware& hardware;
};
