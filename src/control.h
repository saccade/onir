#pragma once

#define BANDS 16      // first eight spots unused. (https://i2cdevices.org/addresses)

#include "client.h"

class Control {
public:
  Control(const Hardware& hardware = no_hardware);
  Control(int* channels, int count, const Hardware& hardware = no_hardware);

  void check();
  int count() {return count_;}
  Client* local() {return clients[local_];}

  Client* clients[BANDS];

private:
  int count_ = 0;
  const int local_ = 0;
};


  
