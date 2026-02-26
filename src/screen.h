#pragma once

#include "display.h"
#include "control.h"
#include "log.h"

#define WIDTH 256

// Splits a string across a handful of displays and keeps them refreshed.
class Screen {

public:
  Screen(const Hardware& hardware = no_hardware) {
    control = new Control(hardware)
    clear();
  }
  
  Screen(Control* control, const Hardware& hardware = no_hardware)
    : control_(control), hardware(hardware) {
    clear();
  }

  void clear() {
    erase();
    if (fan_start_) {
      fan();
    }
    for (Client* client : control_->clients) {
      if (client) {
        client->dial.zero();
      }
    }
  }

  void fan_start(bool b) {
    fan_start_ = b;
  }
  
  void erase() {
    for (int i = 0; i < WIDTH; i++) {
      display_[i] = 0;
      positions_[i] = 0;
    }
    message_ = 0;
    index_ = 0;
  }
    
  void display(const char* message) {
    message_ = message;
    record();
    present();
  }

  void pan(int move) {
    index_ += move;
    present();
  }

  void seek(int index) {
    index_ = index;
    present();
  }

  int index() const { return index_; }
  int width() const { return length(display_); }

  void present() {
    for (int channel = 0; channel < BANDS; channel++) {
      if (Client* client = control_->clients[channel]) {
        for (int i = 0; i < 4; i++) {
          int position = positions_[channel];                      // channel position
          position += i;                                           // digit offset
          position += index_;                                      // pan offset
          if (control_->clients[LOCAL]) {
            position += control_->clients[LOCAL]->dial.value();    // control offset
          }
          if (channel != LOCAL) {
            position += client->dial.value();                      // device offset
          }
          // late-read pattern restricts use of dial value to this spot. (no branches below.)
          client->display.put_char(i, at(position));
        }
      }
    }
  }
  
private:
  char at(int index) {
    return display_[mod(width(), index)];;
  }
  
  void fan() {  // Sequence channel offsets from zero.
    index_ = 0;
    for (int channel = 0; channel < BANDS; channel++) {
      positions_[channel] = 0;
    }
    int count = 0;
    for (int channel = 0; channel < BANDS; channel++) {
      if (control_->clients[channel]) {
        positions_[channel] = 4 * count++;
      }
    }
    present();
  }

  int record() {                                // may pad start/end:
    int receipt = length(message_);             // find the 0
    int length = 0;
    if (receipt and message_[0] != ' ') {
      display_[length++] = ' ';                 // left-pad
    }
    for (int i = 0; (i < receipt) and (length < WIDTH - 1);) {
      display_[length++] = message_[i++];       // copy message
    }
    while (length < 4) {                        // pad to 4
      display_[length++] = ' ';
    }
    if (display_[length-1] != ' '  and length < WIDTH - 2) {
      display_[length++] = ' ';                 // right-pad
    }      
    display_[length] = 0;                       // terminate
    return length;
  }

  static int mod(int modulus, int value) {   // oh, C++...
    int ret = value % modulus;
    if (ret < 0) { ret += modulus; }
    return ret;
  }

  static int length(const char* str) {
    int length = 0;
    while (str and str[length]) { length++; }
    return length;
  }
  
  Control* const control_;
  const char* message_;
  char display_[WIDTH];
  int index_;
  int positions_[BANDS];
  bool fan_start_ = true;

  const Hardware& hardware;  
};
