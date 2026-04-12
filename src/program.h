#pragma once
#include "timing.h"
#include "motor/operation.h"

class Program {
public:

  Instruction instruction;  // client writes here by Wire.
  operator bool() {
    return instruction;
  }

  Operation*& operator[](Cue cue) {
    return operations[(int)cue];
  }
  Operation* operations[(int)Cue::count] = { };
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
