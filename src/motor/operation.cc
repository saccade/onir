#include "operation.h"

Operation::Operation(Cue cue) {
  cue = cue;
  extend();
}

Operation::Operation(const Instruction& instruction) : Instruction(instruction) { }

Command Operation::extend(const Instruction& instruction) {
  if (instruction.direction == Cue::query) return Command::idle;
  const Motion& motion = instruction.motion;
  if (not instruction or not motion) return Command::idle;
  Motion*& location = operator[](motion.motor);
  if (not location) {
    location = new Motion;
  }
  *location = motion;
  return Command::modify;
}

Command Operation::extend() {
  return extend(*this);
}
