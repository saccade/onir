#pragma once

#include "hardware.h"

#define MIN_CHANNEL 8

struct DisplayState {
  char chars[4];  // characters on display; ' ' is blank.
  int point;      // using values outside [0,3] is pointless.
};

struct ServoState {
  int servo_l = 1500;
  int servo_r = 1500;
};
 
struct DialState {
  long count = 0;
  long down_count = 0;
  bool button = false;
};

struct IOState {
  int channel = -1;
  DisplayState display;
  DialState dial;
  ServoState servo;
};

class Control;
class Screen;

class Onir {
public:
  Onir(int* channels, int count, const Hardware& hardware = no_hardware);

  void Onir::display(char* message);

  void update();
  int step();

  Control* control;
  Screen* screen;

private:
  const Hardware& hardware;
  void act();
  bool go();
  const int step_ms_ = 1000;
  int step_ = -1;
};

