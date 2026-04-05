#pragma once

#include "timing.h"

#define PROGRAM_SIZE 16
#define ACTION_SIZE 5  // Number of Motions allowed per Action

enum class Command : u_small {
  none, //
  create,     // add new cue (possibly based on existing cue)
  modify,     // add or modify motion for cue
  perform,    // showtime
  forget,

  condition,  // NOT IMPLEMENTED (place condition on cue)
};

static Command done(Command& command) {
  Command copy = command;
  command = Command::none;
  return copy;
}

static Command nope(Command& command) {
  command = Command::none;
  return command;
}

struct Instruction {
  s_small channel = UNSET;

  Command command = Command::none;  // instruction to driver
  operator bool() const {
    return command != Command::none;
  }

  Cue cue = Cue::go;                // engage motor cue
  Motion motion;
  Cue direction = Cue::stop;        // next action / command argument

  Message message;                  // displays
  Reading reading;                  // sensors

};

// extends Instruction with multi-joint motions
class Action : public Instruction {
public:
  Action();

  void forget();

  // bool modify(Motion motion) {
  //   if (not motion) return false;
  //   for (int i = 0; i <= n_motions; i++) {
  //     if (motion.motor == motions[i].motor) {
  //       motions[i] = motion;
  //     }
  //     return true;
  //   }
  //   if (n_motions >= ACTION_SIZE) return false;
  //   motions[n_motions++] = motion;
  //   return true;
  // }

  Motion motions[ACTION_SIZE] = { };
private:
  int n_motions = 0;
};

class Program {
public:

  Instruction instruction;  // client writes here by Wire.

  void clear(const Cue cue);

  int n_actions = 0;
  Action actions[PROGRAM_SIZE] = { };
};


template <typename T>
using Execute = int (*)(Program& program, Resource<T>& resource);

template <typename T>
static int follow(Rhythm& rhythm, Execute<T> execute, Program& program, Resource<T>& resource) {
  keep(rhythm);
  if (go(rhythm)) {
    rhythm.last = rhythm.now;

    int result = execute(program, resource);
    if (result) {
      rhythm.missed = 0;
    } else {
      rhythm.missed++;
    }

    return result;
  }

  return 0;
}
