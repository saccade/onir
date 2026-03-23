#include "program.h"

Action::Action() { }

void Action::clear() {
  n_motions = 0;
  instruction = Instruction();
}

void Program::clear(const Cue cue) {
  for (Action& action: actions) {
    if (action.instruction.cue == cue) {
      action.clear();
    }
  }
}
