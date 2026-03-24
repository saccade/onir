#pragma once

#include "motor.h"
#include "hardware.h"
#include "program.h"
#include "timing.h"

const int PULSE_NEUTRAL = 1500;
const int PULSE_HALF_SPAN = 500;

// converts motor pitch (signed 8 bit integer) into servo pulse in usec (16 bit)
int servo_pulse(s_small pitch);

class Servo;

static int execute(const Program& program, Robot robot) {
  return 0;
}


class MotorDevice {
public:
  MotorDevice(const Hardware& hardware = no_hardware);

  void engage(Function function);
  void release(Function function);
  void set_pulse(Function function, int usec);

  void update();
  void halt();

  int move();

private:

  Program program;
  const Hardware& hardware;
  int servo_pulses[(int)Function::COUNT] = { };
  bool engaged[(int)Function::COUNT] = { };
  long end_millis[(int)Function::COUNT] = { };

  Servo* robot[(int)Function::COUNT] = { };

  Rhythm rhythm;
};
