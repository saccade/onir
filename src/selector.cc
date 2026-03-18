#include "selector.h"

#include "Arduino.h"

const int min_channel = 0x08;        // = 8 -- https://i2cdevices.org/addresses
const int max_channel = 0x77;        // 0x77 = 119

int selected_channel = min_channel;

Selector::Selector(bool button, const Hardware& hardware) :
  hardware(hardware), button_down(button) {
  dial = new Dial(hardware);
  display = new Display(hardware);
}

Selector::Selector(Dial* dial, Display* display, bool button, const Hardware& hardware)
  : dial(dial), display(display), button_down(button), hardware(hardware) {
  button_down = false;
}

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
  char hx1 = hex_digit(selected_channel / 16);
  char hx2 = hex_digit(selected_channel % 16);

  display->put('0', 0);
  display->put('x', 1);
  display->put(hx1, 2);
  display->put(hx2, 3);
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
  return dial->value() + dial->down_value();
}

bool Selector::done() {
  return dial->release();
}

void Selector::echo() {
  for (int i = 0; i < 4; i++) {
    Serial.print(display->state.chars[i]);
  }
}
