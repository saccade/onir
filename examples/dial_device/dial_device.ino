#include "Wire.h"

#include "onir.h"
#include "dial_device.h"
#include "uno_io.h"

DialDevice* dial;
DialState state;

Hardware hardware = {};

const int I2C_ADDRESS = 8;

long count = 0;
long down_count = 0;
bool button = false;

void on_request() {
  Wire.write((byte*)&state, sizeof(DialState));
}

void setup() {
  Serial.begin(9600);
  Serial.println("dial start");

  uno_io(hardware);
  dial = new DialDevice(hardware);

  Serial.print("i2c address: ");
  Serial.println(I2C_ADDRESS);

  Wire.begin(I2C_ADDRESS);
  Wire.onRequest(on_request);
}

void loop() {
  dial->read(state);
  if (state.count != count || state.down_count != down_count || state.button != button) {
    Serial.print("c: ");
    Serial.print(count = state.count);
    Serial.print(" d: ");
    Serial.print(down_count = state.down_count);
    Serial.print(" b: ");
    Serial.println(button = state.button = state.button);
  }
}
