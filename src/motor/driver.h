#pragma once

#include "motor/trimmer.h"
#include "motor/machine.h"

class Driver {
public:

  Driver(Machine&);

  static Command execute(Program& program, Machine& machine);
  Command update();

private:

  Program program;
  Machine& machine;
};
