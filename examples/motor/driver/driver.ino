#include "Wire.h"

#include "circuits.h"
#include "motor/driver.h"
#include "log.h"

Hardware hardware = {};
Driver* driver{};

Instruction instruction{};

void on_receive(int message_size) {
  Wire.readBytes((byte*)&instruction, sizeof(Instruction));
}

void on_request() {
  Wire.write((byte*)&instruction, sizeof(Instruction));
}

const int channel = 0x09;

void setup() {
  Serial.begin(9600);
  uno_car(hardware);
  driver = new Driver(hardware);

  Serial.print("driver (channel ");
  Serial.print(channel);

  Wire.begin(channel);
  Wire.onReceive(on_receive);
  Wire.onRequest(on_request);

  Serial.print("start (~");
  Serial.print(gamut<Instruction>());
  Serial.println(" free):");
}

void loop() {
  if (instruction) {
    driver->follow(instruction);
    print_instruction(instruction);
  } else {
    driver->drive();
  }
}
