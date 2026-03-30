#include "program.h"
#include "uno_io.h"

#include "motor/motor_device.h"
#include "dial/dial.h"

Hardware hardware = {};
MotorDevice* motor;
Dial* dial;
Motion run;
bool running = false;

void setup() {
  Serial.begin(9600);
  Serial.println("begin");
  uno_io(hardware);
  motor = new MotorDevice(hardware);
  dial = new Dial(hardware);
  motor->engage(Function::MOTOR_MAIN, Target::rotation);
  run.motor = Function::MOTOR_MAIN;
  run.pitch = 64;
  run.winks = 0;
}

void loop() {
  dial->update();
  if (dial->press()) {
    Serial.println("press");
    running = not running;
    if (running) {
//      Serial.println(run.pitch);
      motor->assign(run);

    } else {
      motor->halt();
    }
  }
  motor->advance(Function::MOTOR_MAIN);
}
