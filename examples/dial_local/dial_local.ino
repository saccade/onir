#include "dial.h"
#include "dial_device.h"
#include "uno_io.h"

Dial dial;
DialDevice* device;
int dial_value;
Hardware hardware = { };

void setup() {
  Serial.begin(9600);
  uno_io(hardware);
  device = new DialDevice(hardware);
  dial.attach(device);
  Serial.println("starting dial test");
  dial_value = dial.value();
}

void loop() {
  dial.update();
  if (dial_value != dial.value()) {
    Serial.println(dial.value());
    dial_value = dial.value();
  }
  if (dial.press()) {
    Serial.println("zeroing (" + String(millis()) +")");
    dial.zero();
  }
}
