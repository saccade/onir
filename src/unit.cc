#include "unit.h"
#include "log.h"
#include "dial_device.h"

#include "Arduino.h"

Unit::Unit(int channel) : dial(channel), display(channel) { }
  
Unit::Unit(const Hardware& hardware) : dial(hardware), display(hardware) { }

void Unit::update() {
  dial.update();
  display.refresh();
}


