#include "Wire.h"

#include "program.h"
#include "circuits.h"

#include "motor/motor_device.h"
#include "motor/trimmer.h"
#include "dial/dial.h"
#include "display/display.h"

Hardware hardware = {};
MotorDevice* motor;
Dial* dial_l;
Dial* dial_r;
Display* display;
Trimmer* trimmer_l;
Trimmer* trimmer_r;
Motion run;
bool running = false;

void setup() {
  Serial.begin(9600);
  Serial.println("begin");
  uno_car(hardware);
  motor = new MotorDevice(hardware);
  dial_l = new Dial(hardware);
  dial_r = new Dial(hardware);
  dial_r->set_clock(Function::CLOCK_2);
  dial_r->set_data(Function::DATA_2);
  dial_r->set_switch(Function::SWITCH_2);
  display = new Display(9);
  motor->engage(Function::MOTOR_L_WHEEL, Target::rotation);
  motor->engage(Function::MOTOR_R_WHEEL, Target::rotation);
  trimmer_l = new Trimmer(dial_l, motor, Function::MOTOR_L_WHEEL, 0, display);
  trimmer_r = new Trimmer(dial_r, motor, Function::MOTOR_R_WHEEL, 0, display, true);
  Wire.begin();  // client
}

void loop() {
  trimmer_l->update();
  trimmer_r->update();
}
