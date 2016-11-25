#ifndef __megaphone_h_
#define __megaphone_h_


#include "Arduino.h"
#include "Component.h"
#include "Stormio.h"

#define SHOUT_PERIOD_MILLIS 5000
#define PACKET_TYPE_CAPABILITY 0x7f

class Megaphone {
  public:
    Megaphone(Stormio *, Component *, char);
    void shout();

  private:
    unsigned long lastShoutTimeMillis;
    String description;
    Stormio *stormio;
};

#endif
