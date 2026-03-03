#include "control.h"

#include "Arduino.h"

Control::Control(const Hardware& hardware) {
  if (not empty(hardware)) {
    units[local_] = new Unit(hardware);
    return;
  }
  Serial.println("Control needs channels or hardware. (expect a crash.)");
  delay(1000);
}

Control::Control(int* channels, int ct, const Hardware& hardware) {
  count_ = ct;
  for (int i = 0; i < BANDS; i++) {
    units[i] = 0;
  }

  for (int i = 0; i < count_; i++) {
    int channel = channels[i];
    units[channel] = new Unit(channel);
  }
  if (not empty(hardware)) {
    units[local_] = new Unit(hardware);
  }
}

void Control::check() {
  for (Unit* unit : units) {
    if (unit) {
      unit->update();
    }
  }
}
