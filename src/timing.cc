#include "timing.h"

#include "Arduino.h"

void keep(Rhythm& rhythm) {
  rhythm.now = millis();
}
