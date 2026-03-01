#include "control.h"

#include "Arduino.h"

Control::Control(const Hardware& hardware) {
  if (not empty(hardware)) {
    clients[local_] = new Client(hardware);
    return;
  }
  Serial.println("Control needs channels or hardware. (expect a crash.)");
  delay(1000);
}

Control::Control(int* channels, int ct, const Hardware& hardware) {
  count_ = ct;
  for (int i = 0; i < BANDS; i++) {
    clients[i] = 0;
  }

  for (int i = 0; i < count_; i++) {
    int channel = channels[i];
    clients[channel] = new Client(channel);
  }
  if (not empty(hardware)) {
    clients[local_] = new Client(hardware);
  }
}

void Control::check() {
  for (Client* client : clients) {
    if (client) {
      client->update();
    }
  }
}
