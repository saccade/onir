#pragma once

#include "hardware.h"
#include "dial/dial.h"
#include "display/display.h"

#include "motor_device.h"



class Trimmer {
public:
  Trimmer(Dial* dial, MotorDevice* device, Function = Function::MOTOR_MAIN,
          s_small pitch = 0, Display* display = 0, bool reversed = false);

  s_small pitch();

  void update();

private:
  Reading reading { };

  Dial* dial = 0;
  MotorDevice* device = 0;

  Motion run { };

  bool reversed = false;
  bool pitch_down();
  bool pitch_up();
};
