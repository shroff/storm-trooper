#include "CommandReader.h"

CommandReader::CommandReader(HardwareSerial *serial, String terminator) {
  this->serial = serial;
  this->terminator = terminator;
}

void CommandReader::read(void (*processCommand)(String, String *, int)) {
  if (Serial1.available() > 0) {
    char c = Serial1.read();
    if (c == ' ' || c == '\n' || c == '\r') {
      if (command.length() > 0) {
        if (command == terminator) {
          if (cmdIndex > 0) {
            processCommand(cmd[0], &cmd[1], cmdIndex - 1);
          }
          cmdIndex = 0;
        } else if (cmdIndex < CMD_SIZE) {
          cmd[cmdIndex++] = command;
        } else {
          Serial.print("Dropped command: ");
          Serial.println(command);
        }
        command = "";
      }
    } else if (c != '\r') {
      // Ignore CR
      command += c;
    }
  }
}
