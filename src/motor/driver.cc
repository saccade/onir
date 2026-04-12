#include "driver.h"
#include "log.h"

#include "Arduino.h"

bool logging = true;

void ll(String str) {
  if (logging) {
    Serial.println(str);
  }
}

Driver::Driver(Machine& machine) : machine(machine) {

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
      Serial.println("Driver::drive");
      if (not motion) return reject(todo);                    // can't drive nothing

      if (not joint->trimmer) zero(joint, program);           // set up trimmer
      Command response = joint->drive(todo);                  // update joint setting
      Serial.print("drive: joint response: ");
      Serial.println((int)response);
      if (performative(response) and
          command == Command::perform) {                      // activate joint
        Serial.print("drive: assign -> ");
        mark(todo, machine.assign(todo.motion));              // pass to machine
        Serial.print("new pitch: ");
        Serial.println(todo.motion.pitch);
        return sign(todo, Command::perform);
      }

      Serial.println("drive: modify&sign");
      return sign(todo);;                        // updated drive motion
    }

    Operation* operation_ = program[cue];              // preprogrammed operation
    ll("cue");
    if (not operation_) {
      if (not motion) {
        return reject(todo);                     // don't know how to do nothing
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

  // if (command == Command::condition) {
  //   return (todo);
  // }

  return error(todo);
}

Command Driver::drive(Instruction& todo) {
  program.instruction = todo;
  Command ret = drive();
  return mark(todo, ret);
}

Command Driver::drive() {
  Command response = drive(program, machine);
  machine.update();
  return mark(program.instruction, response);
}
