#pragma once

#include "onir.h"

// abstract interface for a seven-segment display

#define DISPLAY_DEVICE_CHANNEL 9  // 8 & 9 are just for your stuff.
#define MIN_CHANNEL 8             // (https://i2cdevices.org/addresses)
#define DISPLAY_SHOW_ZERO true

class DisplayDevice;

// Client for seven-segment display.
class Display {
public:
  Display(const Hardware& hardware = no_hardware);
  
  Display(int ch);

  void init(int channel);

  void attach(DisplayDevice* d) {
    device = d;
  }
  
  void set_value(int value) {
    set_digits(value);
  }

  void set_point(int place) {
    state.point = place;
  }

  void clear_point() {
    state.point = -1;
  }

  void put(char ch, int place) {
    state.chars[place] = ch;
  }

  void put_str(const char* str) {  // beyond 4 dropped
    for (int i = 0; i < 4; i++) {
      put(str[i], i);
    }
  }
  
  void clear() {
    for (int i = 0; i < 4; i++) {
      put(' ', i);
    }
    clear_point();
  }

  // call in loop()
  void refresh();

  // leave this alone.
  DisplayState state;

private:
  DisplayDevice* device = 0;
  
  int channel = -1;  // don't call if nothing will answer. (the modem rings SYN for a minute.)

  const int UPDATE_FREQ_HZ = 50;
  const int UPDATE_MILLIS = 1000 / UPDATE_FREQ_HZ;
  long last_update = -1;

  void set_digits(int nnn) {
    for (int i = 0; i < 4; i++) {
      put(0, i);  // 0 gives a blank display
    }
    if ((nnn == 0) && !DISPLAY_SHOW_ZERO) return;
    int sign = nnn ? nnn / absv(nnn) : 0;
    if (sign < 0) {
      put('-', 0);
    }
    int value = absv(nnn);
    // assign value's lowest three place values to the display's digits.
    // higher place values are discarded.
    for (int k = 3; k >= 1; k--) {
      int modulus = 1;  // C++, y u no (int) pow?
      for (int j = 0; j < 4 - k - 1; j++) {
        modulus *= 10;
      }
      put('0' + (value / modulus) % 10, k);
      if (!(value / (modulus * 10))) return;
    }
  }

  int absv(int n) {
    if (n > 0) return n;
    if (n == 0) return 0;
    return -n;
  }
  
  void send_update();
  void update_local();
};

