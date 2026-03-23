#include "onir.h"
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

  IOState local_;  // for logging; use dial and display directly.
};
