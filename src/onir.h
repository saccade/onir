#pragma once

#include "program.h"

#define MIN_CHANNEL 8

class Control;
class Screen;

class Onir {
public:
  Onir(int* channels, int count, const Hardware& hardware = no_hardware);

  void Onir::display(char* message);
  void update();

  int now();

  Control* control;
  Screen* screen;

private:
  const Hardware& hardware;
  void act();
  bool go();
  const int step_ms_ = 1000;
  int now_ = UNSET;
};
