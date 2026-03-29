#include "io_device.h"

IODevice::IODevice(const Hardware& hardware) : hardware(hardware) {
  dial = new DialDevice(hardware);
  display = new DisplayDevice(hardware);
  buffer.message.clear();
}
