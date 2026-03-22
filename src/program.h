#pragma once

#include "action.h"

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

struct Action {
  Cue cue = Cue::go;
  Cue next = Cue::stop;
  Motion motion;
};

struct Instruction {
  Command command;
  Cue cue = Cue::stop;

  Cue argument = Cue::stop;  // value can be used by command

  Action action;
  u_small priority = 0;  // unset; priority 1 == high priority
  Message message;

  s_small channel = -1;
  Reading reading;
};

class Program {
public:
  static void clear(Instruction instruction) {
    
  }
private:
  Instruction instructions[128] = { };

};

struct IOState {
  s_small channel = -1;
  Message display;
  Reading dial;
  Action robot;
};
