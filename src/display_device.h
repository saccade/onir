#pragma once

#include "onir.h"

#include "display.h"

const int N_CHAR_MASKS = 128;  // space for all possible segment-sets.

using Fn = Function;

// Runs a 7 segment common cathode clock display.
class DisplayDevice {

public:
  DisplayDevice(const Hardware& hw = no_hardware);

  void update(const DisplayState s) {
    state = s;
  }

  void refresh();
  void clear();
  
  DisplayState state;
  
private:

  // Map positions to position/cathode pins.
  Function positions[4];

  // segment/anode pins
  Function segments[7];

  Hardware segment_masks;
  int sm(char seg) {
    return segment_masks[(int)Fn::DD_A + (int)seg - (int)'A'];
  }

  void set_segment_masks() {
    for (int i = 0; i < interface; i++) {
      segment_masks[i] = 0;
    }
    for (int i = (int)Fn::DD_A; i <= (int)Fn::DD_G; i++) {
      segment_masks[i] = 1 << ((int)Fn::DD_G - i);
    }
  }

  /*
  segment names:
  
      AAAAA
    FF     BB
    FF     BB
      GGGGG
    EE     CC
    EE     CC PP
      DDDDD   PP
  */

  int char_masks[N_CHAR_MASKS];

  void set_char_mask(char ch, char segments[]);

  void set_char_masks() {
    for (int i = 0; i < N_CHAR_MASKS; i++) {
      char_masks[i] = 0;
    }
    char_masks[(int)'0'] = sm('A') + sm('B') + sm('C') + sm('D') + sm('E') + sm('F');
    char_masks[(int)'1'] = sm('B') + sm('C');
    char_masks[(int)'2'] = sm('A') + sm('B') + sm('D') + sm('E') + sm('G');
    char_masks[(int)'3'] = sm('A') + sm('B') + sm('C') + sm('D') + sm('G');
    char_masks[(int)'4'] = sm('B') + sm('C') + sm('F') + sm('G');
    char_masks[(int)'5'] = sm('A') + sm('C') + sm('D') + sm('F') + sm('G');
    char_masks[(int)'6'] = sm('A') + sm('C') + sm('D') + sm('E') + sm('F') + sm('G');
    char_masks[(int)'7'] = sm('A') + sm('B') + sm('C');
    char_masks[(int)'8'] = sm('A') + sm('B') + sm('C') + sm('D') + sm('E') + sm('F') + sm('G');
    char_masks[(int)'9'] = sm('A') + sm('B') + sm('C') + sm('D') + sm('F') + sm('G');
    char_masks[(int)'-'] = sm('G');

    // calligraphic order
    char_masks[(int)'a'] = sm('A') + sm('B') + sm('C') + sm('E') + sm('F') + sm('G');
    char_masks[(int)'b'] = sm('F') + sm('E') + sm('D') + sm('C') + sm('G');
    char_masks[(int)'c'] = sm('G') + sm('E') + sm('D');
    char_masks[(int)'d'] = sm('B') + sm('C') + sm('D') + sm('E') + sm('G');
    char_masks[(int)'e'] = sm('A') + sm('F') + sm('E') + sm('D') + sm('G');
    char_masks[(int)'f'] = sm('A') + sm('F') + sm('E') + sm('G');
    char_masks[(int)'g'] = sm('G') + sm('F') + sm('A') + sm('B') + sm('C') + sm('D');
    char_masks[(int)'h'] = sm('F') + sm('E') + sm('G') + sm('C');
    char_masks[(int)'i'] = sm('E');
    char_masks[(int)'j'] = sm('B') + sm('C') + sm('D') + sm('E') ;
    char_masks[(int)'k'] = sm('F') + sm('E') + sm('G') + sm('B');
    char_masks[(int)'l'] = sm('F') + sm('E') + sm('D');
    char_masks[(int)'m'] = sm('E') + sm('A') + sm('C');
    char_masks[(int)'n'] = sm('E') + sm('G') + sm('C');
    char_masks[(int)'o'] = sm('G') + sm('E') + sm('D') + sm('C');
    char_masks[(int)'p'] = sm('G') + sm('B') + sm('A') + sm('F') + sm('E');
    char_masks[(int)'q'] = sm('G') + sm('F') + sm('A') + sm('B') + sm('C');
    char_masks[(int)'r'] = sm('E') + sm('G');
    char_masks[(int)'s'] = sm('F') + sm('G') + sm('C') + sm('D');
    char_masks[(int)'t'] = sm('D') + sm('E') + sm('F') + sm('G');
    char_masks[(int)'u'] = sm('B') + sm('C') + sm('D') + sm('E') + sm('F');
    char_masks[(int)'v'] = sm('E') + sm('D') + sm('C');
    char_masks[(int)'w'] = sm('F') + sm('D') + sm('B');
    char_masks[(int)'x'] = sm('G') + sm('B') + sm('C');
    char_masks[(int)'y'] = sm('F') + sm('G') + sm('B') + sm('C') + sm('D');
    char_masks[(int)'z'] = sm('A') + sm('B') + sm('G') + sm('E');

    // people will use capitals instead -- do what they mean.
    for (char ch = 'a'; ch <= 'z'; ch++) {
      char_masks[(int)ch + (int)'A' - (int)'a'] = char_masks[(int)ch];
    }

    char_masks[(int)'='] = sm('D') + sm('G');
  }

  const int ms_per_digit = 1;
  const int ms_per_cycle = 4 * ms_per_digit;
  int position_showing = -1;
  long refresh_ms = -1;

  int position_to_show();
  void set_fn_pin(Function fn, bool val);

  // TODO: rename to fn_high? fn_active? (anode/cathode aware)
  void pin_high(Function fn);
  void pin_low(Function fn);
  void set_point_pin(bool val);


  const Hardware& hardware;
};



