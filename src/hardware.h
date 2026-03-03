#pragma once

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

    // servo
    SERVO_L,
    SERVO_R,
    // SERVO_3,

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
    COUNT,  // preserve trailing comma (dr. kraemer is old.)
  
    };

const int interface = (int)Function::COUNT;
using Hardware = int[interface];
extern const Hardware no_hardware;

int assign(Hardware hardware, Function fn, int pin);
int dispatch(const Hardware& hardware, Function fn);
bool empty(const Hardware&);
