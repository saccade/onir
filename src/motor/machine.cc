#include "machine.h"

#include "Arduino.h"
#include "Servo.h"

const int PULSE_NEUTRAL = 1500;

// TODO: add custom per-servo trim (data file?)
static int servo_pulse(s_small pitch) {
  // // bit shift by 7 ==> divide by 2^7.
  // long magnitude = ((long)pitch * (long)PULSE_HALF_SPAN);
  // return PULSE_NEUTRAL + (magnitude >> 7);

  return PULSE_NEUTRAL + pitch * 4;  // (this is fine 4 now.)
}

const int PULSE_MIN = servo_pulse(-127);
const int PULSE_MAX = servo_pulse(127);

const int millis_per_wink = 100;  // a wink is half a blink.
long end_millis(u_small winks) {
  if (winks == 0) return UNSET;
  return millis() + winks * millis_per_wink;
}

Machine::Machine(const Hardware& hardware) : hardware(hardware) { }

Joint* Machine::engage(Function function, Target target, s_small pitch) {
  if (not joints[function]) {
    Joint* joint = new Joint();
    joint->servo = new Servo;
    joint->target = target;
    joint->servo->attach(dispatch(hardware, function));

    joint->pulse_usec = servo_pulse(pitch);
    joint->target_usec = servo_pulse(pitch);
    joint->servo->write(joint->pulse_usec);
    joint->rhythm.group = int(function);
    joint->rhythm.layoff = 100;  // quick as a wink

    joint->max_delta = 3;

    joints[function] = joint;

  }
}

void Machine::release(Function function) {
  Joint* joint = joints[function];
  if (joint) {
    Servo* servo = joints[function]->servo;
    joint->servo = 0;
    joints[function] = 0;
    servo->detach();
    delete servo;
    delete joint;
  }
}

static void control(Joint* joint, Motion motion) {
  if (not joint) return;

  joint->target_usec = servo_pulse(motion.pitch);
  Serial.print("joint->target_usec: ");
  Serial.println(joint->target_usec);
  joint->end_millis = end_millis(motion.winks);
}

void Machine::assign(Motion motion) {
  Joint* joint = joints[motion.motor];
  if (joint) {
    control(joint, motion);
  }
}

int Machine::advance(Function function) {
  Joint* joint = joints[function];
  if (not joint) return 0;

  const int delta = pulse_delta(*joint);
  if (not delta) return 0;

  joint->pulse_usec += delta;
  // Serial.print(millis());
  // Serial.print(":");
  // Serial.println(joint->pulse_usec);
  joint->servo->write(joint->pulse_usec);
  return delta;
}

void Machine::update() {
  for (Function fn = Function::MOTOR_MAIN; fn < Function::MOTOR_END; fn++) {
    advance(fn);
  }
}

int Machine::slam(Function function) {
  Joint* joint = joints[function];
  if (not joint) return 0;

  const int delta =  - joint->pulse_usec;
  if (not delta) return 0;

  joint->pulse_usec = joint->target_usec;
  joint->servo->write(joint->pulse_usec);
  return delta;
}

void Machine::halt() {
  for (Function fn = Function::MOTOR_MAIN; fn < Function::MOTOR_END; fn++) {
    halt(fn);
  }
}

static bool stop_seek(Joint* joint) {
  Serial.println("stop_seek");
  if (not joint) return false;

  if (joint->target != Target::position) return false;

  joint->target_usec = joint->pulse_usec;  // try to stop here
  return true;
}

static bool stop_spin(Joint* joint) {
  if (not joint) return false;
  if (joint->target != Target::rotation) return false;
  joint->target_usec = PULSE_NEUTRAL;  // try to spin down
  return true;
}

static bool hold(Joint* joint) {

  if (not joint->servo) return false;
  if (joint->target == Target::position) return stop_seek(joint);
  if (joint->target == Target::rotation) return stop_spin(joint);
}

void Machine::halt(Function fn) {
  Joint* joint = joints[fn];
  if (joint) {
    hold(joint);
  }
}
