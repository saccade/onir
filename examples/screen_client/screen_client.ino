#include "onir.h"
#include "uno_io.h"
#include "log.h"

#include "Wire.h"

const int count = 5;
int channels[count] = { 8, 9, 10, 11, 12 };

Onir* onir;
Hardware hardware = {};

IOState state;
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
