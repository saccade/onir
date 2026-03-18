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

static int Dial::request(Order& order) {
  if (order.channel < MIN_CHANNEL) return 0;
  
  Wire.requestFrom(order.channel, order.to_read);
  
  if (Wire.available() == order.to_read) {
    Wire.readBytes(order.buffer, order.to_read);
    return order.to_read;
  }
  return 0;
}

int Dial::request() {
  return request(order);
}

void Dial::update() {
  if (device) {
    device->read(state);
  } else {
    follow(rhythm, request, order);
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
