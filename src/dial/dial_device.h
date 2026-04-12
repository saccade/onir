#pragma once

#include "data.h"

class DialDevice {
public:
  DialDevice(const Hardware& hardware);
  void take(Reading& reading);

  void set_clock(Function fn);
  void set_data(Function fn);
  void set_switch(Function fn);

private:
  bool clock = true;  // this pin is HIGH by default

  bool clock_pin() const;
  bool data_pin() const;
  bool switch_pressed() const;

  Function clock_fn = Function::CLOCK;
  Function data_fn = Function::DATA;
  Function switch_fn = Function::SWITCH;

  const Hardware& hardware;
};
