#include "Wire.h"

#include "onir.h"
#include "dial_device.h"
#include "uno_io.h"

DialDevice dial;
DialState state;

hardware = { };

const int I2C_ADDRESS = 8;

long last_count = 0;
bool last_button = false;

void on_request() {
  Wire.write((byte*)&state, sizeof(DialState));
}

void setup() {
  Serial.begin(9600);
  Serial.println("dial dial start");
  Serial.print("i2c address: ");
  Serial.println(I2C_ADDRESS);
  uno_io(hardware);
  dial = Dial(hardware);

  Wire.begin(I2C_ADDRESS);
  Wire.onRequest(on_request);
}

void loop() {
  dial.read(state);
  if (state.count != last_count || state.button != last_button) {
    Serial.print("count: ");
    Serial.print(state.count);
    Serial.print("  button: ");
    Serial.println(state.button);

    last_count = state.count;
    last_button = state.button;
  }
}
