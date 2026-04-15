#include "driver.h"
#include "log.h"

#include "Arduino.h"

bool logging = true;

Driver::Driver(Machine& machine) : machine(machine) {
  init();
}

Driver::Driver(const Hardware& hardware) : machine(*(new Machine(hardware))) {
  machine.engage_hardware(Target::rotation);
  init();
}

void Driver::init() {
  program[Cue::drive] = new Operation(Cue::drive);

  Operation* stop = new Operation(Cue::stop);
  Operation* go = new Operation(Cue::go);

  program[Cue::stop] = stop;
  program[Cue::go] = go;

  for (Function motor = Function::MOTOR_MAIN; motor < Function::MOTOR_END; motor++) {
    Motion* halt =  new Motion();
    halt->motor = motor;
    (*stop)[motor] = halt;

    Motion* move =  new Motion();
    move->motor = motor;
    move->pitch = 1 << 5;
    (*go)[motor] = move;
  }
}

static Command Driver::drive(Program& program, Machine& machine) {
  Instruction& todo = program.instruction;
  if (not todo) {
    return reject(todo);                                    // nothing.
  }

  const Motion& motion = todo.motion;
  Function function = motion.motor;
  if (motion and not is_motor(function)){
    return reject(todo);                                    // nonsense
  }

  Joint* joint = machine[function];
  if (not joint) return reject(todo, Command::missing);     // don't have one of those

  Command& command = todo.command;
  Cue cue = todo.cue;

  if (command == Command::perform or command == Command::modify) {
    if (cue == Cue::none) {
      if (not motion) return reject(todo);                    // nothing to perform/modify

      return mark(todo, machine.assign(motion));              // do the motion
    }

    if (cue == Cue::drive) {                                  // manual mode
      if (not motion) return reject(todo);                    // can't drive nothing

      if (not joint->trimmer) zero(joint, program);           // set up trimmer
      Command response = joint->drive(todo);                  // update joint setting
      if (performative(response) and
          command == Command::perform) {                      // activate joint
        return sign(todo, machine.assign(todo.motion));       // pass to machine
      }

      return sign(todo);                                // updated drive motion
    }

    Operation* operation_ = program[cue];               // preprogrammed operation
    if (not operation_) {
      if (not motion) {
        return reject(todo);                            // don't know how to do nothing
      } else {
        operation_ = new Operation(todo);
      }
    }

    Operation& operation = *operation_;
    Function motor = machine.assign(operation);   // queried or first found
    todo.motion = *operation[motor];
    todo.direction = operation.direction;
    todo.reading = operation.reading;
    todo.message = operation.message;
    return sign(todo);
  }

  if (command == Command::copy) {
    Cue direction = todo.direction;
    todo.direction = Cue::none;
    program[direction] = new Operation(todo);
  }

  return error(todo);
}

Command Driver::follow(Instruction& todo) {
  program.instruction = todo;
  Command result = drive();
  todo = program.instruction;
  return result;
}

Command Driver::drive() {
  Command response {};
  if (program) {
    Command response = drive(program, machine);
  }
  machine.advance();
  return mark(program.instruction, response);
}
