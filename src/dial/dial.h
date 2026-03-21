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
    return state.count - zero_offset;
  }
  
  int down_value() const {
    return state.down_count - down_offset;
  }

  bool button() const {
    return state.button;
  }

  void zero() {
    zero_offset = state.count;
    down_offset = state.down_count;
    change.buffer = (char*)&state;
    change.to_read = (int)sizeof(DialState);
  }

  DialState state;

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
