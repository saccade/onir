#pragma once

#include "onir.h"
#include "timing.h"

class DialDevice;

class Dial {
public:
  Dial(const Hardware& hardware);
  Dial(int channel);

  void attach(DialDevice* device);
  
  void set_channel(int channel) {
    rhythm.channel = channel;
    change.channel = channel;
  }

  int channel() {
    return rhythm.channel;
  }

  void update();
  bool press();
  bool release();

  int value() const {
    return reading.count - zero_offset;
  }
  
  int down_value() const {
    return reading.down_count - down_offset;
  }

  bool button() const {
    return reading.button;
  }

  void zero() {
    zero_offset = reading.count;
    down_offset = reading.down_count;
    change.buffer = (char*)&reading;
    change.to_read = (int)sizeof(Reading);
  }

  Reading reading;

private:
  int call();
  static int call(Change& change);
  
//  int channel_ = -1;  // -1 means unset
  int zero_offset = 0;
  int down_offset = 0;
  
  bool press_ready = false;
  bool release_ready = false;

  DialDevice* device = nullptr;
  const Hardware& hardware;
  Rhythm rhythm;
  Change change;
};
