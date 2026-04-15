#pragma once

#include "hardware.h"

struct Reading {
  int count = 0;
  int down_count = 0;
  bool button = false;

  bool operator==(const Reading& other) const {
    return (count == other.count and
            down_count == other.down_count and
            button == other.button);

  };

  bool operator!=(const Reading& other) const {
    return not operator==(other);

  };

  bool operator<(const Reading& other) const {
    if (count != other.count) {
      return count < other.count;
    }
    if (down_count != other.down_count) {
      return down_count < other.down_count;
    }
    return false;
  }

  bool operator>(const Reading& other) const {
    if (count != other.count) {
      return count > other.count;
    }
    if (down_count != other.down_count) {
      return down_count > other.down_count;
    }
    return false;
  }
};

struct Message {
  char chars[4] = {0, 0, 0, 0} ;  // characters on display
  s_small point = UNSET;          // values outside [0,3] are pointless.

  void clear() {
    for (int i = 0; i < 4; i++) {
      chars[i] = ' ';
    }
  }

};

struct Motion {  // TODO: remove default ctor; require a motor(function).
  Function motor = Function::NONE;
  operator bool() const {
    return motor != Function::NONE;
  }

  s_small pitch = 0;
  u_small winks = 10;  // try for a second

  void clear() {
    motor = Function::NONE;
    pitch = 0;
    winks = 0;
  }

};

enum class Cue : u_small {
  none,   // unset
  stop,   // static responses for motor type ("stay there" or "don't spin")
  go,     // default action

  // act
  forward,
  back,
  spin_clockwise,
  spin_counterwise,
  go_right,
  go_left,
  back_right,
  back_left,

  // follow
  drive,   // set pitch by dial
  scan,    // watch IR sensor

  // modify
  invert,  // flip dial meaning
  revert,  // normal reading

  query,   // ask about motor motion

  count,   // last item used for size
};

inline bool is_motor(Cue cue) {
  int i = (int)cue;
  return (i >= (int)Cue::stop and i <=(int)Cue::scan);
}

enum class Command : u_small {
  none,       //
  perform,    // showtime

  modify,     // add or modify motion for cue
  copy,       // duplicate cue (uses .direction)
  forget,     // delete object

  idle,       // driver indicates no action
  missing,    // driving indicates lack of requested joint
  reject,     // driver indicates bad instruction
  error,      // logical exception in device (layer 8)

  // condition,  // NOT IMPLEMENTED (place condition on cue)

};

static inline bool imperative(const Command& command) {  // sensible request?
  if (command == Command::none)    return false;

  if (command == Command::idle)    return false;
  if (command == Command::reject)  return false;
  if (command == Command::missing) return false;
  if (command == Command::error)   return false;

  return true;
}



static inline bool performative(const Command& response) {  // need motor update?
  if (response == Command::perform) return true;
  if (response == Command::modify)  return true;
  return false;
}

static inline bool responsive(const Command& command) {  // active response?
  if (command == Command::none)    return false;

  if (command == Command::idle)    return true;
  if (command == Command::missing) return true;
  if (command == Command::reject)  return true;
  if (command == Command::error)   return true;

  return false;
}

static bool informative(const Command& response) {  // valid response?
  return response != Command::none;
}

struct Instruction {
  s_small channel = UNSET;

  Command command = Command::none;    // boss to driver
  Command respond = Command::none;    // driver to boss

  // response codes:
  //
  // {(c)ommand; (r)espond}

  // INVALID (but still boolean-false)
  // {c= none;       r = none}      == misformed. (why send this?)

  // TRUE values (there could be something to do):

  // incoming:
  // {c = <active>;  r = none}      == incoming (from client)
  //
  // processing:
  // {c = <active>;  r = reject}    == illogical request (unsigned)
  // {c = <active>;  r = <other>}   == processing (mid-task)

  // FALSE values (nothing to do here):

  // done:
  // {c = none;      r = <active>}  == took an action (expected)

  // apt requests:
  // {c= none;       r = idle}      == no action needed
  // {c= idle;       r = idle}      == no action needed -- rate signal

  // bad requests:
  // {c = reject     r = none}      == no request made
  // {c = reject;    r = idle}      == over-rate error
  // {c = reject     r = missing}   == missing needed hardware
  // {c = reject     r = error}     == bad state -- logic error in device
  // {c = reject;    r = <other>}   == unreasonable request

  // returns true if instruction needs action:
  operator bool() const {
    if (imperative(command)) return true;
    if (not informative(respond)) return true;
    return false;
  }

  Cue cue = Cue::go;          // engage motor cue
  Motion motion;

  Message message;            // displays
  Reading reading;            // sensors

  Instruction* next = 0;      // next intruction; if none, try direction
  Cue direction = Cue::none;

  void clear() {
    Instruction blank;
    *this = blank;
  }
};

static inline bool invalid(const Instruction& todo) {
  return todo.command == Command::none and todo.respond == Command::none;
}

static inline bool rejected(const Instruction& todo) {
  return todo.command == Command::reject;
}

static inline bool performed(const Instruction& todo) {
  return todo.command == Command::none and todo.respond == Command::perform;
}

static inline bool modified(const Instruction& todo) {
  return todo.command == Command::none and todo.respond == Command::modify;
}

static inline bool copied(const Instruction& todo) {
  return todo.command == Command::none and todo.respond == Command::copy;
}

static inline bool forgotten(const Instruction& todo) {
  return todo.command == Command::none and todo.respond == Command::forget;
}

static inline bool succeeded(const Instruction& todo) {
  return todo.respond == Command::none and imperative(todo.respond);
}

static inline bool idled(const Instruction& todo) {

  return ((todo.command == Command::none or todo.command == Command::idle) and
          todo.respond == Command::idle);
}

static inline Instruction& perform(Instruction& todo) {
  todo.command = Command::perform;
  todo.respond = Command::none;
  return todo;
}

static inline Instruction& modify(Instruction& todo) {
  todo.command = Command::modify;
  todo.respond = Command::none;
  return todo;
}

static inline Instruction& copy(Instruction& todo) {
  todo.command = Command::copy;
  todo.respond = Command::none;
  return todo;
}

static inline Instruction& forget(Instruction& todo) {
  todo.command = Command::forget;
  todo.respond = Command::none;
  return todo;
}

// sensible request, but no action was needed.
static Command idle(Instruction& todo) {
  todo.respond = Command::idle;
  todo.command = Command::none;  // TODO: set rate-limit flag values here
  return todo.command;
}

static inline bool completed(const Instruction& todo) {
  return succeeded(todo) or idled(todo);
}

// return response value; let go of todo unaltered.
static Command release(Instruction& todo, Command response) {
  return response;
}

// let go of todo unaltered; empty response.
static Command release(Instruction& todo) {
  return Command::none;
}

// set and return response field from response; release instruction.
static Command mark(Instruction& todo, Command response) {
  todo.respond = response;
  return release(todo, response);
}
static Command apply(Command response, Instruction& todo) {
  todo.respond = response;
  return release(todo, response);
}

static Command reject(Instruction& todo, Command response) {
  todo.respond = response;
  todo.command = Command::reject;
  return todo.command;
}

static Command reject(Instruction& todo) {
  todo.respond = todo.command;
  todo.command = Command::reject;
  return todo.command;
}

static Command sign(Instruction& todo) {
  if (not imperative(todo.command) or not informative(todo.respond))
    return reject(todo);

  Command command = todo.command;
  todo.command = Command::none;
  return mark(todo, command);
}

static Command sign(Instruction& todo, Command response) {
  todo.respond = response;
  return sign(todo);
}

static Command error(Instruction& todo) {
  return reject(todo, Command::error);
}
