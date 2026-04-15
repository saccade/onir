#include "Wire.h"

#include "circuits.h"

#include "data.h"
#include "motor/control.h"

#include "log.h"

Hardware hardware = {};
Control* control = {};

const int channel = 0x0A;

void setup() {
  Serial.begin(9600);
  uno_car(hardware);
  Wire.begin();   // client mode
  control = new Control(channel, hardware);
  Serial.print("start loop (~");
  Serial.print(gamut<Instruction>());
  Serial.println(" free):");
}

void loop() {
  control->update();
}
