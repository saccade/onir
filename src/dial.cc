#include "dial.h"
#include "dial_device.h"

#include "Arduino.h"
#include "Wire.h"

Dial::Dial(const Hardware& hardware) {
  device = new DialDevice(hardware);
  zero();
}

Dial::Dial(int ch) {
  zero();
  set_channel(ch);
}

void Dial::attach(DialDevice* d) {
  device = d;
}

void Dial::update() {
  if (device) {
    device->read(state);
    return;
  }
  if (channel_ >= MIN_CHANNEL) {
    Wire.requestFrom(channel_, (int)sizeof(DialState));
    if (Wire.available() == sizeof(DialState)) {
      Wire.readBytes((char*)&state, sizeof(DialState));
    }
    return;
  }

  // Not configured: no device, no channel
  if (millis() % 1000 == 0) {
    Serial.println("Dial not configured.");
  }
}

bool Dial::press() {
  if (press_ready and state.button) {
    press_ready = false;
    return true;
  }
  if (not press_ready and not state.button) {
    press_ready = true;
  }
  return false;
}

bool Dial::release() {
  if (release_ready and not state.button) {
    release_ready = false;
    return true;
  }
  if (not release_ready and state.button) {
    release_ready = true;
  }
  return false;
}
