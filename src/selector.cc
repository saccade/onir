#include "selector.h"

#include "Arduino.h"

int selected_channel = min_channel;
  const int min_channel = 0x08;        // = 8 -- https://i2cdevices.org/addresses
  const int max_channel = 0x77;        // 0x77 = 119
  // note: 192 - 8 = 184 = 8x23 => 32x23 char 'display'


Selector::Selector(const Hardware& hardware) : hardware(hardware) {
  button_down = false;
  selected_channel = min_channel;
}

Selector::Selector(Dial* dial, Display* display, bool button=false, const Hardware& hardware)
  : dial(dial), display(display), button_down(button), hardware(hardware) { }

void Selector::channel_up() {
  if (selected_channel < max_channel) {
    selected_channel++;
  } else {
    Serial.print("max channel = ");
    echo();
  }
}

void Selector::channel_down() {
  if (selected_channel > min_channel)  {
    selected_channel--;
  } else {
    Serial.println();
    Serial.print("min channel = ");
    echo();
  }
}

char Selector::hex_digit(int value) {
  if ((value < 0) or (value > 16)) return '-';
  if (value < 10) return '0' + value;
  return 'A' + value - 10;
}

void Selector::display_channel() {
  display->put_char(0, '0');
  display->put_char(1, 'x');
  display->put_char(2, hex_digit(selected_channel / 16));
  display->put_char(3, hex_digit(selected_channel % 16));
}


int Selector::get_channel() {
  Serial.print("channel: ");
  display_channel();
  long value = dial->value();
  dial->update();
  while (not done()) {
    dial->update();
    if (count() != value) {
      if (count() > value) {
        channel_up();
      }
      if (count() < value) {
        channel_down();
      }
      value = count();
      
      display_channel();
      echo();
      Serial.print("? ");
    }
    display->refresh();
  }
  echo();
  Serial.println(".");
  Serial.print("channel ");
  Serial.println(selected_channel);
  dial->zero();
  return selected_channel;  
}

int Selector::count() {
  if (button_down) {
    Serial.println(button_down);
    return dial->down_value();
  } else {
    return dial->value();
  }
}

bool Selector::done() {
  if (button_down) {
    return dial->release();
  } else {
    return dial->press();
  }
}

void Selector::echo() {
  for (int i = 0; i < 4; i++) {
    Serial.print(display->state.chars[i]);
  }
}
