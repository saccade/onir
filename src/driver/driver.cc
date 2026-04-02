#include "driver.h"
#include "motor/motor_device.h"

static Command Driver::execute(Program& program, MotorDevice& device) {
  Instruction& todo = program.instruction;
  Command& command = todo.command;
  switch (command) {

  case Command::none: { }

  case Command::perform: {
    bool trigger = false;
    for (const Action& action : program.actions) {
      if (action and action.cue == todo.cue) {
        for (const Motion& motion: action.motions) {
          device.assign(motion);
          trigger = true;
        }
      }
    }
    return (trigger) ? Command::perform : Command::none;
  }

  case Command::modify: {
    for (const Action& action : program.actions) {
      if (action and action.cue == todo.cue) {
        for (const Motion& motion: action.motions) {
          if (motion.motor == todo.motion.motor) {
            motion = todo.motion;
            return Command::modify;
          }
        }
      }
    }
    return Command::none;

  }
  case Command::create: {
    // see if it already exists
    for (const Action& action : program.actions) {
      if (action.cue == todo.cue) {
        return Command::none;
      }
    }

    // find a free spot (there
  }
  case Command::condition: {
  }
  }
  return done(command);
}
