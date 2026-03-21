#include "dial.h"
#include "dial_device.h"

#include "Arduino.h"
#include "Wire.h"

Dial::Dial(const Hardware& hardware) {
  device = new DialDevice(hardware);
  zero();
}

Dial::Dial(int channel) {
  zero();
  set_channel(channel);
}

void Dial::attach(DialDevice* device) {
  device = device;
}

static int Dial::call(Change& change) {
  if (change.channel < MIN_CHANNEL) return 0;
  
  Wire.requestFrom(change.channel, change.to_read);
  
  if (Wire.available() == change.to_read) {
    Wire.readBytes(change.buffer, change.to_read);
    return change.to_read;
  }
  return 0;
}

int Dial::call() {
  return call(change);
}

void Dial::update() {
  if (device) {
    device->read(state);
  } else {
    follow(rhythm, call, change);
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
