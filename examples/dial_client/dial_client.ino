#include "Wire.h"

#include "onir.h"
#include "dial.h"

Dial* dial;

const int I2C_ADDRESS = 8;

void setup() {
  Serial.begin(9600);
  Serial.println("dial client " + String(I2C_ADDRESS));

  Wire.begin();  // client mode
  dial = new Dial(I2C_ADDRESS);
}

void loop() {

  long last_value = dial->value();
  long last_down_value = dial->down_value();
  dial->update();

  if (dial->value() != last_value) {
    Serial.println("v: " + String(dial->value()));
  }

  if (dial->down_value() != last_down_value) {
    Serial.println("dv: " + String(dial->down_value()));
  }

  if (dial->press()) {
    Serial.println("press");
  }

  if (dial->release()) {
    Serial.println("release");
  }
}
