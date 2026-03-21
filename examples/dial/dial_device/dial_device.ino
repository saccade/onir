#include "Wire.h"

#include "onir.h"
#include "dial/dial_device.h"
#include "uno_io.h"

DialDevice* dial;
Reading reading;

Hardware hardware = {};

const int I2C_ADDRESS = 8;

long count = 0;
long down_count = 0;
bool button = false;

void on_request() {
  Wire.write((byte*)&reading, sizeof(Reading));
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
  dial->read(reading);
  if (reading.count != count || reading.down_count != down_count || reading.button != button) {
    Serial.print("c: ");
    Serial.print(count = reading.count);
    Serial.print(" d: ");
    Serial.print(down_count = reading.down_count);
    Serial.print(" b: ");
    Serial.println(button = reading.button = reading.button);
  }
}
