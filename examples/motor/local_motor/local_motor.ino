#include "circuits.h"

#include "motor/driver.h"
#include "dial/dial.h"

#include "log.h"

Hardware hardware = {};
Driver* driver{};
Operation* drive{};

Instruction drive_l{};
Dial* dial_l{};

Instruction drive_r{};
Dial* dial_r{};

void update_side(Dial* dial, Instruction& instruction) {
  Reading& reading = dial->update();
  if (reading != instruction.reading) {
    instruction.reading = reading;
    driver->follow(instruction);
    print_instruction(instruction);
  }
}

void setup() {
  Serial.begin(9600);
  uno_car(hardware);
  driver = new Driver(hardware);
  dial_l = new Dial(hardware);
  dial_r = new Dial(hardware);
  dial_r->set_dial_2();

  // driving instructions
  drive_l.command = Command::perform;
  drive_l.cue = Cue::drive;
  drive_l.reading.count = UNSET;
  drive_r = Instruction(drive_l);

  drive_l.motion.motor = Function::MOTOR_L_WHEEL;
  drive_r.motion.motor = Function::MOTOR_R_WHEEL;

  Serial.print("start loop (~");
  Serial.print(gamut<Instruction>());
  Serial.println("B free):");
}

void loop() {
  update_side(dial_l, drive_l);
  update_side(dial_r, drive_r);
  driver->drive();
}
