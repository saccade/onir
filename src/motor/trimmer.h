#pragma once

#include "motor/program.h"

class Trimmer {
public:
  Trimmer(const Reading&, Motion* motion, bool invert = false);

  s_small pitch();

  // checks and handles correctly-addressed instructions
  Command execute(Instruction& todo);

private:

  const Reading& reading;
  bool read_dial();  // doesn't check anything, assumes good reading
  Reading prior;     // copy needed to spot changes

  Motion* motion;

  bool invert = false;

  bool pitch_down();
  bool pitch_up();
};
