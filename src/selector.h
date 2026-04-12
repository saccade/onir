#pragma once

#include "data.h"

#include "dial/dial.h"
#include "display/display.h"

extern int selected_channel;

// uses a dial and display to select a numerical I2C channel (int).
class Selector {
public:
  Selector(Dial* dial, Display* display, bool button=false, const Hardware& hardware = no_hardware);

  Selector(bool button = false, const Hardware& hardware = no_hardware);

  void set_button(bool button) { button_down = button; }

  int get_channel();


private:
  void channel_up();
  void channel_down();

  Dial* dial = nullptr;
  Display* display = nullptr;

  bool button_down;
  bool done();
  int count();

  void display_channel();
  void echo();

  const Hardware& hardware;
};
