#include "control.h"

#include "Arduino.h"

Control::Control(const Hardware& hardware = no_hardware) : hardware(hardware) {
  
}

Control::Control(int* channels, int ct, const Hardware& hardware) : hardware(hardware) {
  count_ = ct;
  for (int i = 0; i < BANDS; i++) {
    clients[i] = 0;
  }

  for (int i = 0; i < count_; i++) {
    int channel = channels[i];
    clients[channel] = new Client(channel, hardware);
  }
}

void Control::update() {
  for (Client* client : clients) {
    if (client) {
      client->update();
    }
  }
}
