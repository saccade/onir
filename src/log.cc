#include "log.h"
#include "unit.h"

#include "Arduino.h"
#include "string.h"

int wink() { return millis() / 100; }  // a blink is 1/5 s. a wink is half a blink.
// int log_winks = 600;  // give me a minute to think.

int log_winks = 10; // I need a second.

void log(const IOState& s) {
  static long log_time_winks = -log_winks;  // persists across calls
  if (wink() - log_time_winks > log_winks) {
    log_time_winks = wink();
    print_io(s);
  }
}

static char safe(char c) {
  if (c == 0) return '0';
  if (c < 32 || c == 127) return '\\';
  return c;
}

static long checksum(const IOState& s) {
  const byte* p = (const byte*)&s;
  long sum = 0;
  for (int i = 0; i < (int)sizeof(IOState); ++i)
    sum += p[i];
  return sum;
}


static bool prints(char c) {
  return (c == 0 or (c >= 32 and c < 127));
}

void print_display(const DisplayState& display) {
  bool printable = true;
  for (int i = 0; i < 4; ++i) {
    if (!prints(display.chars[i])) {
      printable = false;
      break;
    }
  }

  if (printable) {
    Serial.print("\"");
    char buf[5];
    for (int i = 0; i < 4; ++i) {
      char ch = display.chars[i];
      buf[i] = ch ? ch : ' ';
    }
    buf[4] = '\0';
    Serial.print(buf);
    Serial.print("\"");
    return;
  }

  // mixed / non-printable case
  Serial.print('{');
  for (int i = 0; i < 4; ++i) {
    if (prints(display.chars[i])) {
      Serial.print('\'');
      Serial.print(display.chars[i]);
      Serial.print('\'');
    } else {
      Serial.print(String((int)(byte)display.chars[i]));
    }

    if (i != 3) Serial.print(", ");
  }
  Serial.print('}');
}

void left_pad(int v) {
  // left pad to width 4 incl sign
  if (v >= 0) Serial.print(' ');
  if (v > -100 && v < 100) Serial.print(' ');
  if (v > -10  && v < 10)  Serial.print(' ');
}

void log_id() {
  static int log_id_ = 0;
  Serial.print('#');
  Serial.print(log_id_++);
}

void print_io(const IOState& state) {
  Serial.print(" {c: ");
  Serial.print(state.channel);
  Serial.print(", s: {m: ");
  print_display(state.display);
  Serial.print(", p: ");
  Serial.print(state.display.point);
  Serial.print("}, d: {v: ");

  int v = state.dial.count;
  left_pad(v);
  Serial.print(v);
  Serial.print("; d: ");
  int d = state.dial.down_count;
  left_pad(d);
  Serial.print(d);

  Serial.print("; b: ");
  Serial.print(state.dial.button ? 1 : 0);
  Serial.print("}, #: ");
  Serial.print(checksum(state));
  Serial.print(" } (m: ");
  Serial.print(millis());
  Serial.println(")");
}

// local copy of dial and display contents (not definitive -- just read the dial!)
void mirror_device_units(Unit* unit) {
  unit->local_.channel = unit->dial.channel();
  memcpy(&unit->local_.dial,   &unit->dial.state,   sizeof(unit->dial.state));
  memcpy(&unit->local_.display, &unit->display.state, sizeof(unit->display.state));
}

void log_io(Unit* unit) {
  log_id();
  if (not unit) {
    Serial.println("0");
    return;
  }
  mirror_device_units(unit);
  print_io(unit->local_);
}
