#pragma once

#include "onir.h"

class DialDevice;

class Dial {
public:
  Dial(const Hardware& hardware);
  Dial(int ch);

  void attach(DialDevice* device);
  
  void set_channel(int channel) {
    channel_ = channel;
  }

  int channel() {
    return channel_;
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
  }

  DialState state;

private:
  int channel_ = -1;  // -1 means unset
  int zero_offset = 0;
  int down_offset = 0;
  
  bool press_ready = false;
  bool release_ready = false;

  DialDevice* device = nullptr;
  const Hardware& hardware;
};
