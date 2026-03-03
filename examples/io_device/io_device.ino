// Binds Wire callbacks to addresses in IODevice::state.

#include "Wire.h"

#include "io_device.h"
#include "hardware.h"
#include "uno_io.h"
#include "selector.h"
#include "log.h"

DisplayDevice* device;
Display* display;
Hardware hardware = {};

IODevice* io;

int channel;

void on_receive(int n_bytes) {
  int n = n_bytes;
  if (n > (int)sizeof(DisplayState)) {
    Serial.println("Format error!");
    n = (int)sizeof(DisplayState);
  }
  Wire.readBytes((byte*)&io->state.display, n);
}

void on_request() {
  Wire.write((byte*)&io->state.dial, sizeof(DialState));
}

void start_channel() {
  Wire.end();  // idempotent
  Wire.begin(channel);
  Wire.onReceive(on_receive);
  Wire.onRequest(on_request);
}

void setup() {
  Serial.begin(9600);
  log_winks = 10;  // I need a second.
  Serial.println("starting io device");
  uno_io(hardware);
  Dial dial(hardware);
  Display display(hardware);
  display.set_point(-1);
  channel = Selector(&dial, &display, false, hardware).get_channel();
  Serial.print("selected: ");
  Serial.println(channel);

  io = new IODevice(hardware);
  start_channel();
}

void loop() {
  io->update();
  log(io->state);
  // if (io.reboot_channel > 0) {
  //   channel = io.reboot_channel;
  //   start_channel();
  // }
}