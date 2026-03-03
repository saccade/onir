#include "onir.h"
#include "display.h"
#include "dial.h"

class Unit {
public:
  Unit(const Hardware& hardware);
  Unit(int channel);

  void update();
  Dial dial;
  Display display;

  IOState local_;  // for logging; use dial and display directly.
};
