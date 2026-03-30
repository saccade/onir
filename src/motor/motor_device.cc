#include "Arduino.h"
#include "Servo.h"

#include "motor_device.h"

// TODO: add custom per-servo trim (data file?)
static int servo_pulse(s_small pitch) {
  // // bit shift by 7 ==> divide by 2^7.
  // long magnitude = ((long)pitch * (long)PULSE_HALF_SPAN);
  // return PULSE_NEUTRAL + (magnitude >> 7);

  return PULSE_NEUTRAL + pitch * 4;  // (this is fine 4 now.)
}

const int millis_per_wink = 100;  // a wink is half a blink.
long end_millis(u_small winks) {
  if (winks == 0) return UNSET;
  return millis() + winks * millis_per_wink;
}

MotorDevice::MotorDevice(const Hardware& hardware) : hardware(hardware) { }

Joint* MotorDevice::engage(Function function, Target target, s_small pitch) {
  if (not robot[function]) {
    Serial.println("engage");
    Joint* joint = new Joint();
    joint->servo = new Servo;
    joint->target = target;
    joint->servo->attach(dispatch(hardware, function));

    Serial.println("dispatch: ");
    Serial.println(hardware[(int)Function::DD_A]);
    Serial.println(dispatch(hardware, function));
    joint->pulse_usec = servo_pulse(pitch);
    joint->target_usec = servo_pulse(pitch);
    joint->servo->write(joint->pulse_usec);
    Serial.println(joint->pulse_usec);
    joint->rhythm.group = int(function);
    joint->rhythm.layoff = 100;  // quick as a wink

    joint->max_delta = 3;

    robot[function] = joint;

  }
}

void MotorDevice::release(Function function) {
  Joint* joint = robot[function];
  if (joint) {
    Servo* servo = robot[function]->servo;
    joint->servo = 0;
    robot[function] = 0;
    servo->detach();
    delete servo;
    delete joint;
  }
}

void control(Joint* joint, Motion motion) {
  if (not joint) return;

  joint->target_usec = servo_pulse(motion.pitch);
  Serial.println("joint->target_usec");

  Serial.println(joint->target_usec);
  joint->end_millis = end_millis(motion.winks);
  Serial.println(joint->pulse_usec);
}

void MotorDevice::assign(Motion motion) {
  Serial.println("assign");
  Joint* joint = robot[motion.motor];
  if (joint) {
    Serial.print("found ");
    Serial.println((int)motion.motor);
    control(joint, motion);
  }
}

int MotorDevice::advance(Function function) {
  Joint* joint = robot[function];
  if (not joint) return 0;

  const int delta = pulse_delta(*joint);
  if (not delta) return 0;

  joint->pulse_usec += delta;
  Serial.print(millis());
  Serial.print(":");
  Serial.println(joint->pulse_usec);
  joint->servo->write(joint->pulse_usec);
  return delta;
}

int MotorDevice::slam(Function function) {
  Joint* joint = robot[function];
  if (not joint) return 0;

  const int delta =  - joint->pulse_usec;
  if (not delta) return 0;

  joint->pulse_usec = joint->target_usec;
  joint->servo->write(joint->pulse_usec);
  return delta;
}

static bool stop_seek(Joint& joint) {
  if (not joint) return false;

  if (joint.target != Target::position) return false;

  joint.target_usec = joint.pulse_usec;  // try to stop here
  return true;
}

static bool stop_spin(Joint& joint) {
  if (not joint) return false;

  if (joint.target != Target::rotation) return false;

  joint.target_usec = PULSE_NEUTRAL;  // try to spin down
  return true;
}

static bool hold(Joint& joint) {
  Serial.println("hold");
  if (not joint.servo) return false;

  if (joint.target == Target::position) return stop_seek(joint);

  if (joint.target == Target::rotation) return stop_spin(joint);
}

void MotorDevice::halt() {
  for (Function fn = Function::MOTOR_MAIN; fn < Function::MOTOR_END; fn++) {
    Joint* joint = robot[fn];
    if (joint) {
      hold(*joint);
    }
  }
}
