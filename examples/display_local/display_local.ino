#include "display.h"
#include "display_device.h"
#include "uno_io.h"
#include "hardware.h"

Hardware hardware = { };

Display display;
DisplayDevice* device;

void banner() {
  display.put_str("onir");
  display.set_point(1);
  Serial.println("onir");
}

void setup() {
  Serial.begin(9600);
  uno_io(hardware);
  device = new DisplayDevice(hardware);
  display.attach(device);
  banner();
}

void loop() {
  display.refresh();
}
