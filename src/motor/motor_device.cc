#include "motor_device.h"

// TODO: add custom per-servo trim (data file?)
int servo_pulse(s_small pitch) {
  // bit shift by 7 ==> divide by 2^7.
  long magnitude = ((long)pitch * (long)PULSE_HALF_SPAN);
  return PULSE_NEUTRAL + (magnitude >> 7);
}
