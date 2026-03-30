#pragma once

using u_small = unsigned char;
using s_small = signed char;

#define UNSET -1


const extern int min_channel;
const extern int max_channel;

// names for logical pin functions.

enum class Function : u_small {
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
  MOTOR_MAIN,
  MOTOR_L_WHEEL,
  MOTOR_R_WHEEL,
  MOTOR_BASE,
  MOTOR_SHOULDER,
  MOTOR_ELBOW,
  MOTOR_WRIST,
  MOTOR_HAND,
  MOTOR_END,

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


inline Function& operator++(Function& fn) {
  Function ret = fn;
  fn = (Function)((int)fn + 1);
  return ret;
}

class Servo;

using Hardware = int[(int)Function::COUNT];
extern const Hardware no_hardware;
extern const int hardware_size;

int assign(Hardware hardware, Function fn, int pin);
int dispatch(const Hardware& hardware, Function fn);
bool empty(const Hardware& hardware);
void clear(Hardware hardware);

template <typename T>
struct Resource {
  T hardware[(int)Function::COUNT] { };

  T& operator[](Function f) {
    return hardware[(int)f];
  }

  const T& operator[](Function f) const {
    return hardware[(int)f];
  }
};
