#include "trimmer.h"

#include "Arduino.h"


Trimmer::Trimmer(Dial* dial, MotorDevice* device, Function fn,
                 s_small pitch, Display* display, bool reversed) :
  dial(dial), device(device), reversed(reversed) {
  run.motor = fn;
  run.pitch = pitch;
  run.winks = 0;  // no set end time
}

s_small Trimmer::pitch() {
  return run.pitch;
}

void Trimmer::update() {
  Reading last = dial->reading;
  dial->update();
  if (dial->press()) {
    run.pitch = 0;
    device->assign(run);
  } else {
    if (last < dial->reading) {
      (reversed) ? pitch_up() : pitch_down();
    } else if (last > dial->reading) {
      (reversed) ? pitch_down() : pitch_up();
    }
  }
  device->update();
}

bool Trimmer::pitch_down() {
  if (run.pitch == (-1 << 7) + 1) {  // -127
    Serial.println(run.pitch);
    return false;
  }
  run.pitch--;
  device->assign(run);
  return true;
}

bool Trimmer::pitch_up() {
  if (run.pitch == (1 << 7) - 1) {  // 127
    Serial.println(run.pitch);
    return false;
  }
  run.pitch++;
  device->assign(run);
  return true;
}
