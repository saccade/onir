#include "uno_io.h"

void uno_io(Hardware hardware) {

  for (int fn = (int)Fn::NONE + 1; fn < (int)Fn::COUNT - 1; fn++) {
    assign(hardware, (Function)fn, -1); // it's rude to yell at the modem
  }

  // digit display
  assign(hardware, Fn::DD_A, 6);
  assign(hardware, Fn::DD_B, 2);
  assign(hardware, Fn::DD_C, 10);
  assign(hardware, Fn::DD_D, 12);
  assign(hardware, Fn::DD_E, 13);
  assign(hardware, Fn::DD_F, 5);
  assign(hardware, Fn::DD_G, 9);
  assign(hardware, Fn::DD_P, 11);
  assign(hardware, Fn::DD_1, 7);
  assign(hardware, Fn::DD_2, 4);
  assign(hardware, Fn::DD_3, 3);
  assign(hardware, Fn::DD_4, 8);

  // dial encoder
  
  assign(hardware, Fn::CLOCK, 17);   // A3
  assign(hardware, Fn::DATA, 16);    // A2
  assign(hardware, Fn::SWITCH, 15);  // A1
}

