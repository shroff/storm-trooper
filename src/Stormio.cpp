#include "Stormio.h"

Stormio::Stormio(HardwareSerial *serial, String terminator) {
  this->serial = serial;
  this->terminator = terminator;
  this->cmdIndex = 0;
}

void Stormio::read(void (*processCommand)(String, String *, int)) {
  if (serial->available() > 0) {
    char c = serial->read();
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

void Stormio::write(unsigned char type, String data) {
  unsigned char length = data.length();
  unsigned char checksum = type;
  checksum += length;
  for (int i = data.length() - 1; i >= 0; i--) {
    checksum += data.charAt(i);
  }
  checksum = ~checksum;
  serial->write(checksum);
  serial->write(type);
  serial->write(length);
  serial->print(data);
  serial->flush();
}
