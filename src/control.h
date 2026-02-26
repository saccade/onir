#pragma once

#include "client.h"

#define BANDS 16      // first eight spots unused. (https://i2cdevices.org/addresses)
#define LOCAL 0       // zero = here  

class Control {
public:
  Control(const Hardware& hardware = no_hardware);
  Control(int* channels, int count, const Hardware& hardware = no_hardware);

  void update();
  int count() {return count_;}

  Client* clients[BANDS];
 
private:
  int count_ = 0;
  const Hardware& hardware;
};


  
