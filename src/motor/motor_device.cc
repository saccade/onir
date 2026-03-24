#include "Servo.h"

#include "motor_device.h"

// TODO: add custom per-servo trim (data file?)
int servo_pulse(s_small pitch) {
  // // bit shift by 7 ==> divide by 2^7.
  // long magnitude = ((long)pitch * (long)PULSE_HALF_SPAN);
  // return PULSE_NEUTRAL + (magnitude >> 7);

  return PULSE_NEUTRAL + pitch * 4;  // (this is fine 4 now.)
}

MotorDevice::MotorDevice(const Hardware& hardware) : hardware(hardware) {
  rhythm.layoff = 100;  // quick as a wink
}

void MotorDevice::engage(Function function) {
  Servo* servo = new Servo;
  servo->attach(dispatch(hardware, function));
  robot[(int)function] = servo;
  set_pulse(function, PULSE_NEUTRAL);
}

void MotorDevice::release(Function function) {
  Servo* servo = robot[(int)function];
  robot[(int)function] = 0;
  servo->detach();
  delete servo;
}

void MotorDevice::set_pulse(Function function, int usec) {
  if (robot[(int)function]) {
    robot[(int)function]->write(usec);
  }
}

void MotorDevice::update() {
  // follow(rhythm, &move);
  // follow(rhythm, execute, program, robot);
}


int MotorDevice::move() {
//  for () {}
//  if () {}
  return 0;
}
