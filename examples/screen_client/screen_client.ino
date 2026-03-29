#include "onir.h"
#include "control.h"
#include "log.h"
#include "screen.h"
#include "uno_io.h"
#include "log.h"

#include "Wire.h"

// const int count = 6;
// int channels[count] = { 8, 9, 10, 11, 12, 13 };
const int count = 4;
int channels[count] = { 8, 9, 10, 11 };

Onir* onir;
Hardware hardware = {};

Instruction state;
const char message[64] = "ko labs -- onir --";
//const char message[64] = "1 at a time";


void setup() {
  Serial.begin(9600);
  Serial.println("start");
  log_winks = 25;
  uno_io(hardware);
  onir = new Onir(channels, count, hardware);
  onir->display(message);
  Wire.begin();
}

void loop() {
  onir->update();
}
