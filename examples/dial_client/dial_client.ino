#include "Wire.h"

#include "onir.h"
#include "dial.h"

Dial dial;

const int I2C_ADDRESS = 8;

void setup() {
  Serial.begin(9600);
  Serial.println("dial client " + String(I2C_ADDRESS));

  Wire.begin();  // client mode
  dial.set_channel(I2C_ADDRESS);
}

void loop() {

  long last_value = dial.value();
  dial.update();
  long value = dial.value();

  if (value != last_value) {
    Serial.println("v: " + String(value));
  }

  if (dial.press()) {
    Serial.println("signal");
  }
}
