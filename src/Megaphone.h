#ifndef __megaphone_h_
#define __megaphone_h_

#include "Arduino.h"
#include "Component.h"

#define SHOUT_PERIOD_MILLIS 5000

class Megaphone {
  public:
    Megaphone(Component *, int);
    void shout();

  private:
    unsigned long lastShoutTimeMillis;
    String description;
};

#endif
