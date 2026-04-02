#include "circuits.h"
#include "dial/dial.h"
#include "display/display.h"

Hardware hardware = {};
Dial* dial_l;
Dial* dial_r;
Display* display;

Reading reading_l;
Reading reading_r;

void show(Reading reading, String label = "") {
  if (label) {
    Serial.print(label);
    Serial.print(": ; ");
  }
  Serial.print("v: ");
  Serial.print(Dial::active_value(reading), HEX);
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  Serial.println("begin");
  uno_car(hardware);
  dial_l = new Dial(hardware);
  dial_r = new Dial(hardware);
  display = new Display(9);
  dial_r->set_clock(Function::CLOCK_2);
  dial_r->set_data(Function::DATA_2);
  dial_r->set_switch(Function::SWITCH_2);
}

void loop() {
  dial_l->update();
  dial_r->update();

  if (reading_l != dial_l->reading) {
    reading_l = dial_l->reading;
    show(reading_l, "l");
  }
  if (reading_r != dial_r->reading) {
    reading_r = dial_r->reading;
    show(reading_r, "r");
  }
}
