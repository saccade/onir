#pragma once

#include "motor.h"
#include "hardware.h"

const int PULSE_NEUTRAL = 1500;
const int PULSE_HALF_SPAN = 500;

// converts motor pitch (signed 8 bit integer) into servo pulse in usec (16 bit)
int servo_pulse(s_small pitch);



class MotorDevice {
public:
  MotorDevice(const Hardware& hw = no_hardware);

  void update();
  void halt();
  
};
