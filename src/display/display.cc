#include "display.h"
#include "timing.h"

#include "Arduino.h"
#include "Wire.h"

#include "display_device.h"

Display::Display(const Hardware& hardware) {
  device = new DisplayDevice(hardware);
  clear();
}

Display::Display(int channel) {
  rhythm.group = channel;
  change.channel = channel;
  change.buffer = (char*)&message;
  change.to_write = sizeof(Message);
  clear();
}

static int Display::call(Change& change) {
  if (change.channel < min_channel) return 0;
  Wire.beginTransmission(change.channel);
  Wire.write(change.buffer, change.to_write);
  Wire.endTransmission();
  return 1;
}

int Display::call() {
  return call(change);
}

void Display::refresh() {
  follow(rhythm, call, change);
  if (device) {
    device->update(message);
    device->refresh();
  }
}

String chars(Message message) {
  char result[5];
  for (int i = 0; i < 4; i++) {
    result[i] = (char)message.chars[i];
  }
  result[5] = 0;
  return String(result);
}
