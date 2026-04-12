#include "display/display.h"
#include "dial/dial.h"
#include "motor/motor.h"

class Unit {
public:
  Unit(const Hardware& hardware);
  Unit(int channel);

  void update();
  Dial dial;
  Display display;

  Instruction local_;  // for logging; use dial and display directly.
};
