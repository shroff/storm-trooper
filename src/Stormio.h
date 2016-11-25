#ifndef __stormio_h_
#define __stormio_h_

#include "Arduino.h"

const int CMD_SIZE = 16;

class Stormio {
  public:
    Stormio(HardwareSerial *, String);
    void read(void (*)(String, String *, int));
    void write(unsigned char, String);

  private:
    HardwareSerial *serial;
    String terminator;

    String command = "";
    String cmd[CMD_SIZE];
    int cmdIndex;
};

#endif
