#pragma once

#include "hardware.h"

#define MIN_CHANNEL 8

struct Message {
  char chars[4];       // characters on display
  int point = -1;      // values outside [0,3] are pointless.
};

void blank(Message* message);

struct ServoState {
  int servo_l = 1500;
  int servo_r = 1500;
};
 
struct Reading {
  long count = 0;
  long down_count = 0;
  bool button = false;
};

struct IOState {
  int channel = -1;
  Message display;
  Reading dial;
  ServoState servo;
};

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
  int now_ = -1;
};

