#ifndef __command_reader_h_
#define __command_reader_h_

#include "Arduino.h"

const int CMD_SIZE = 16;

class CommandReader {
  public:
    CommandReader(HardwareSerial *, String);
    void read(void (*)(String, String *, int));

  private:
    HardwareSerial *serial;
    String terminator;

  String command = "";
  String cmd[CMD_SIZE];
  int cmdIndex;


};

#endif
