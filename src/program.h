#pragma once

#include "action.h"

#define PROGRAM_SIZE 128
#define ACTION_SIZE 5  // Number of Motions allowed per Action

struct Reading {
  int count = 0;
  int down_count = 0;
  bool button = false;
};

enum class Command {
  none, //
  perform,    // showtime
  
  create,     // add new cue (possibly based on existing cue)
  extend,     // add new motion to cue
  condition,  // place condition on cue
};

struct Instruction {
  Command command = Command::none;
  Cue cue = Cue::go;

  Cue next = Cue::stop;

  Motion motion;
  u_small priority = 0;  // unset; priority 1 == high priority
  Message message;

  s_small channel = -1;
  Reading reading;
};

// extends Instruction with multi-joint motions
class Action {
public:
  Action();

  void clear();

  bool add_motion(Motion motion) {
    if (n_motions > ACTION_SIZE) return false;
    for (int i = 0; i <= n_motions; i++) {
      if (motion.motor == motions[i].motor) {
        motions[i] = motion;
      }
    }
    motions[n_motions++] = motion;
    return true;
  }

  Instruction instruction;
  Motion motions[ACTION_SIZE] = { };
private:
  int n_motions = 0;
};

class Program {
public:
  void clear(const Cue cue);

private:
  int n_actions = 0;
  Action actions[PROGRAM_SIZE] = { };
};

struct IOState {
  s_small channel = -1;
  Message display;
  Reading dial;
  Action robot;
};
