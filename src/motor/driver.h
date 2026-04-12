#pragma once

#include "motor/trimmer.h"
#include "motor/machine.h"

class Driver {
public:

  Driver(Machine&);

  static Command drive(Program& program, Machine& machine);
  Command drive(Instruction& todo);

  Command drive();
  Command update();

private:

  static bool zero(Joint* joint, Program& program) {
    if (not joint) return false;
    if (joint->trimmer) return true;
    const Instruction& todo = program.instruction;
    if (todo.cue != Cue::drive) return false;
    Operation*& drive = program[Cue::drive];
    if (not drive) {
      drive = new Operation(todo);
    }
    Motion*& motion = (*drive)[todo.motion.motor];
    if (not motion) {
      motion = new Motion(todo.motion);
    }
    joint->trimmer = new Trimmer(todo.reading, motion);
    return true;
  }

  Program program;
  Machine& machine;
};
