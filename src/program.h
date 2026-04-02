#pragma once

#include "hardware.h"

#define PROGRAM_SIZE 16
#define ACTION_SIZE 5  // Number of Motions allowed per Action

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

struct Motion {
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
  stop,   // static action for servo type ("stay there" or "don't spin")
  go,     // active motion

  // actions
  forward,
  back,
  spin_clockwise,
  spin_counterwise,
  go_right,
  go_left,
  back_right,
  back_left,

  // follows
  drive,   // set pitch by dial
  scan,    // watch IR sensor

  count,  // last item used for size
};

static_assert(PROGRAM_SIZE > (int)Cue::count);

enum class Command : u_small {
  none, //
  perform,    // showtime

  modify,     // add or modify motion for cue

  create,     // add new cue (possibly based on existing cue)
  condition,  // place condition on cue
  forget,
};

static Command done(Command& command)  {
  Command copy = command;
  command = Command::none;
  return copy;
}

struct Instruction {
  s_small channel = UNSET;

  Command command = Command::none;  // modify motor cue
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

  bool modify(Motion motion) {
    if (motion.motor == Function::NONE) return false;
    for (int i = 0; i <= n_motions; i++) {
      if (motion.motor == motions[i].motor) {
        motions[i] = motion;
      }
      return true;
    }
    if (n_motions >= ACTION_SIZE) return false;
    motions[n_motions++] = motion;
    return true;
  }

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
