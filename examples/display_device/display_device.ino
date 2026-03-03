#include "display_device.h"
#include "uno_io.h"
#include "hardware.h"

#include "Wire.h"

DisplayDevice* display;
Hardware hardware = { };

void update_display(int message_size) {
  Wire.readBytes((byte*)&display->state, message_size);
}

void setup() {
  Serial.begin(9600);
  Serial.println("starting display.");
  uno_io(hardware);
  display = new DisplayDevice(hardware);

  Wire.begin(DISPLAY_DEVICE_CHANNEL);
  Wire.onReceive(update_display);
}

void loop() {
  display->refresh();
}
