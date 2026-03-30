#pragma once

#include "driver.h"
#include "program.h"

class DriverDevice {

public:
  DriverDevice(const Hardware& hw = no_hardware);

  void update(const Instruction& instruction);

  void refresh();
  void halt();

  const Hardware& hardware;
};
