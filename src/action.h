#pragma once

#include "hardware.h"

struct Message {
  char chars[4] = {0, 0, 0, 0} ;  // characters on display
  s_small point = -1;             // values outside [0,3] are pointless.
};

struct Motion {
  Function motor = Function::NONE;
  s_small pitch = 0;
  u_small duration = 0;
};

enum class Cue {
  stop,
  
  go, // default action to modify

  // actions
  forward,
  back,
  spin_clock,
  spin_counter,
  go_right,
  go_left,
  back_right,
  back_left,

  // follows
  drive,   // set pitch by dial
  scan,    // watch IR sensor
  
  count,  // last item used for size
};
