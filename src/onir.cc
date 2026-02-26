#include "onir.h"
#include "control.h"
#include "screen.h"

#include "Arduino.h"

Onir::Onir(int* channels, int count, const Hardware& hardware) : hardware(hardware) {
  control = new Control(channels, count, hardware);
  screen = new Screen(control, hardware);
}

void Onir::display(char* message) {
  screen->display(message);
}

void Onir::act() {
  if (go()) {
    screen->pan(1);
    if (control->clients[8]) {
      log_io(control->clients[8]);
    }
  }
}

void Onir::update() {
  control->update();
  act();
  screen->present();
}

int Onir::step() {
  return millis() / step_ms_;
}

bool Onir::go() {
  int old_step = step_;
  return old_step != (step_ = step());
}
