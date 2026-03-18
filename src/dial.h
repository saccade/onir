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
    order.channel = channel;
  }

  int channel() {
    return order.channel;
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
    order.buffer = (char*)&state;
    order.to_read = (int)sizeof(DialState);
  }

  DialState state;

private:
  int request();
  static int request(Order& order);
  
//  int channel_ = -1;  // -1 means unset
  int zero_offset = 0;
  int down_offset = 0;
  
  bool press_ready = false;
  bool release_ready = false;

  DialDevice* device = nullptr;
  const Hardware& hardware;
  Rhythm rhythm;
  Order order;
};
