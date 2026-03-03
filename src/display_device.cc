#include "display_device.h"

#include "Arduino.h"
#include "Wire.h"

DisplayDevice::DisplayDevice(const Hardware& hardware) : hardware(hardware) {
  for (int i = 0; i < 7; i++) {
    segments[i] = (Function)(i + (int)Fn::DD_A);
  }
  for (int i = 0; i < 4; i++) {
    positions[i] = (Function)(i + (int)Fn::DD_1);
  }
  set_segment_masks();
  set_char_masks();
  for (int i = (int)Function::DD_A; i <= (int)Function::DD_4; i++) {
    pinMode(dispatch(hardware, (Function)i), OUTPUT);
  }
  clear();
}

void DisplayDevice::clear() {
  for (Function segment : segments) {
    pin_low(segment);
  }
  set_point_pin(LOW);
  for (Function position : positions) {
    pin_high(position);
  }
}

int DisplayDevice::position_to_show() {
  return (millis() % ms_per_cycle) / ms_per_digit;
}

void DisplayDevice::set_fn_pin(Function fn, bool val) {
  digitalWrite(dispatch(hardware, fn), val);
}

void DisplayDevice::pin_high(Function fn) {
  set_fn_pin(fn, HIGH);
}

void DisplayDevice::pin_low(Function fn) {
  set_fn_pin(fn, LOW);
}

void DisplayDevice::set_point_pin(bool val) {
  set_fn_pin(Fn::DD_P, val);
}

void DisplayDevice::refresh() {
  int position = position_to_show();
  if (position != position_showing) {  // redraw display
    position_showing = position;
    clear();
    for (Function segment : segments) {
      if (segment_masks[(int)segment] & char_masks[(int)state.chars[position]]) {
        pin_high(segment);
      }
    }
    if (state.point == position) {
      set_point_pin(HIGH);
    }
    pin_low(positions[position]);
  }
}
