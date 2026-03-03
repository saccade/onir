#include "display.h"

#include "Arduino.h"
#include "Wire.h"

#include "display_device.h"

Display::Display(const Hardware& hardware) {
  device = new DisplayDevice(hardware);
  clear();
}

Display::Display(int channel) : channel(channel) {
  clear();
}

void Display::refresh() {
  if (device) {
    device->refresh();
  }
  if ((long)millis() - last_update > UPDATE_MILLIS) {
    last_update = millis();
    send_update();
    if (device) {
      device->update(state);  
    }
  }
}

String chars(DisplayState state) {
  char result[5];
  for (int i = 0; i < 4; i++) {
    result[i] = (char)state.chars[i];
  }
  result[5] = 0;
  return String(result);
}

// TODO: unify handling for this logic before adding servos.
void Display::send_update() {
  if (channel < MIN_CHANNEL) return;
  Wire.beginTransmission(channel);
  Wire.write((const byte*) &state, (int)sizeof(DisplayState));
  Wire.endTransmission();
}
