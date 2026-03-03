#pragma once

#include "onir.h"

class DialDevice {
public:
  DialDevice(const Hardware& hardware);
  void read(DialState& state);

private:
  bool clock = true;  // this pin is HIGH by default

  bool clock_pin() const;
  bool data_pin() const;
  bool switch_pressed() const;

  const Hardware& hardware;
};
