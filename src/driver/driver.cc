#include "driver.h"
#include "motor/motor_device.h"

static Command Driver::execute(Program& program, Resource<Joint>& robot) {
  Instruction& todo = program.instruction;
  Command& command = todo.command;
  switch (command) {
  case Command::none: { }
  case Command::perform: {
    for (const Action& action : program.actions) {
      if (action and action.cue == todo.cue) {
        for (const Motion& motion: action.motions) {
          if (robot[motion.motor]) {
//            control(robot[motion.motor], motion);
          }
        }
      }
    }
  }

  case Command::extend: {
    for (const Action& action : program.actions) {
      if (action and action.cue == todo.cue) {
        for (const Motion& motion: action.motions) {
          if (motion.motor == todo.motion.motor) {
            motion = todo.motion;
            return Command::extend;
          }
          if (true) {} //xxxx
        }
      }
    }

  }
  case Command::create: {

  }
  case Command::condition: {
  }
  }
  return done(command);
}
