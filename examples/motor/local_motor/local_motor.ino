#include "Wire.h"

#include "circuits.h"
#include "motor/program.h"

#include "motor/driver.h"
#include "dial/dial.h"
#include "display/display.h"


Hardware hardware = {};
Machine* machine{};
Operation* drive{};
Dial* dial_l{};
Dial* dial_r{};
Display* display{};
Driver* driver{};
Motion run{};
Instruction drive_l{};
Instruction drive_r{};
bool running = false;

void update_side(Dial* dial, Instruction& instruction) {
  Reading& reading = dial->update();
  if (reading != instruction.reading) {
    instruction.reading = reading;
    {  // logging
      Serial.print("m: ");
      Serial.print((int)instruction.motion.motor);
      Serial.print("; c: ");
      Serial.print(instruction.reading.count);
      Serial.println();
    }

    driver->drive(instruction);
  }
}

void update() {
  update_side(dial_l, drive_l);
  update_side(dial_r, drive_r);
  driver->drive();
}

void setup() {
  Serial.begin(9600);
  Serial.println("start");
  uno_car(hardware);
  machine = new Machine(hardware);
  driver = new Driver(*machine);
  dial_l = new Dial(hardware);
  dial_r = new Dial(hardware);
  dial_r->set_dial_2();
  display = new Display(9);

  machine->engage_hardware(Target::rotation);

  drive_l.command = Command::perform;
  drive_l.cue = Cue::drive;
  drive_r = Instruction(drive_l);
  Serial.println((int)&drive_l);
  Serial.println((int)&drive_r);
  drive_l.motion.motor = Function::MOTOR_L_WHEEL;
  drive_r.motion.motor = Function::MOTOR_R_WHEEL;

  Serial.print("gamut: ");
  Serial.println(gamut<Instruction>());
}


void loop() {
  update();
}
