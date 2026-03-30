#pragma once

#include "motor.h"
#include "hardware.h"
#include "program.h"
#include "timing.h"

const int PULSE_NEUTRAL = 1500;
// const int PULSE_HALF_SPAN = 500;

// converts motor pitch (signed 8 bit integer) into servo pulse in usec (16 bit)
static int servo_pulse(s_small pitch);

long end_millis(long duration);

enum class Target : u_small {
  position,
  rotation,
  torque
};

struct Joint {
  int pulse_usec = UNSET;
  int target_usec = UNSET;
  long end_millis = UNSET;
  u_small max_delta = 8;  // = beats * 1000 * delta usec / milli
  Target target = Target::position;
  Rhythm rhythm;

  Servo* servo = 0;
  operator bool() const {
    return servo;
  }

};

void control(Joint* joint, Motion motion);

static bool stop_seek(Joint& joint);
static bool stop_spin(Joint& joint);
static bool hold(Joint& joint);

class Servo;

class MotorDevice {
public:
  MotorDevice(const Hardware& hardware = no_hardware);

  Joint* engage(Function function, Target target, s_small pitch = 0);
  void release(Function function);
  void assign(Motion motion);
  // void set_pulse(Function function, int usec, long end_ms = 0);

  void update();  // call in loop()
  void halt();

  int advance(Function function);
  int slam(Function function);

private:

  static int pulse_delta(const Joint& joint) {
    int target_delta = joint.target_usec - joint.pulse_usec;
    if (not target_delta) return 0;
    int sign = (target_delta > 0) ? 1 : -1;

    if (sign * target_delta <= joint.max_delta) {
      return target_delta;
    } else {
      return sign * joint.max_delta;
    }
  }

  Cue active = Cue::stop;

  Program program;

  static Command execute(Program& program, Resource<Joint>& settings);

  const Hardware& hardware;
  Resource<Joint*> robot = {};
};
