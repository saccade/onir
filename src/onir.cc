#include "onir.h"
#include "control.h"
#include "screen.h"

#include "Arduino.h"

Onir::Onir(int* channels, int count, const Hardware& hardware) : hardware(hardware) {
  control = new Control(channels, count, hardware);
  screen = new Screen(control);
}

void Onir::display(char* message) {
  screen->display(message);
}

void Onir::act() {
  if (go()) {
    screen->pan(1);
    log_io(control->local());
  }
}

void Onir::update() {
  control->check();
  act();
  screen->show();
}

int Onir::now() {
  return millis() / step_ms_;
}

bool Onir::go() {
  int past = now_;
  now_ = now();
  return past != now_;
}
