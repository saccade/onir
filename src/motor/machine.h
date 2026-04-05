#pragma once

#include "timing.h"
#include "trimmer.h"

const extern int PULSE_NEUTRAL;
const extern int PULSE_MAX;
const extern int PULSE_MIN;

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

  Trimmer* trimmer = 0;
};

static void control(Joint* joint, Motion motion);

static bool stop_seek(Joint* joint);
static bool stop_spin(Joint* joint);
static bool hold(Joint* joint);

class Servo;

class Machine {
public:
  Machine(const Hardware& hardware = no_hardware);

  Joint* operator[](Function fn) {
    Joint* joint = joints[fn];
    if (not joint) return 0;
    return joint;
  }

  Joint* engage(Function joint, Target target, s_small pitch = 0);

  void release(Function joint);
  void assign(Motion motion);

  void update();  // call in loop()
  void halt(Function joint);
  void halt();

  int advance(Function joint);
  int slam(Function joint);

  Trimmer* trimmer(Function joint);

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

  const Hardware& hardware;
  Resource<Joint*> joints = {};
};
