#pragma once

using u_small = unsigned char;
using s_small = signed char;

// names for logical pin functions.

enum class Function {
  NONE,  // default
  
  // power pair
  GROUND,
  VCC,
  
  // dial
  CLOCK,
  DATA,
  SWITCH,

  CLOCK_2,
  DATA_2,
  SWITCH_2,

  // motor names
  MOTOR_L_WHEEL,
  MOTOR_R_WHEEL,
  MOTOR_BASE,
  MOTOR_SHOULDER,
  MOTOR_ELBOW,
  MOTOR_WRIST,
  MOTOR_HAND,

  // seven segment digit display outputs

  // segments (anode)
  DD_A,
  DD_B,
  DD_C,
  DD_D,
  DD_E,
  DD_F,
  DD_G,
  DD_P,

  // positions (cathode)
  DD_1,
  DD_2,
  DD_3,
  DD_4,

  COUNT,  // last item used for size
  
};

class Servo;

using Hardware = int[(int)Function::COUNT];
using Robot = Servo*[(int)Function::COUNT];
extern const Hardware no_hardware;
extern const int hardware_size;

int assign(Hardware hardware, Function fn, int pin);
int dispatch(const Hardware& hardware, Function fn);
bool empty(const Hardware& hardware);
void clear(Hardware hardware);
