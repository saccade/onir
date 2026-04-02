#pragma once

#include "driver_device.h"

#include "program.h"
#include "motor/motor_device.h"

class Driver {
  static Command execute(Program& program, MotorDevice& device);
};
