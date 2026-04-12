#pragma once

#include "data.h"

// extends Instruction with multi-joint motions
class Operation : public Instruction {
public:
  Operation(Cue);
  Operation(const Instruction&);
  operator bool() {
    return cue != Cue::none;
  }

  Command extend(const Instruction& todo);   // update from todo.motion
  Command extend();                          // update from Action::motion

  Motion* motions[(int)Function::COUNT] = { };
  Motion*& operator[](Function fn) {
    return motions[(int)cue];
  }
};
