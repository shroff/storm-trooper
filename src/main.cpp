#include "Arduino.h"
#include "Strip.h"
#include "CommandReader.h"

// Number of RGB LEDs in strand:
const int STRIP_LED_COUNT = 32;
const int STRIP_DATA_PIN  = 2;
const int STRIP_CLOCK_PIN = 3;
Strip strip = Strip(STRIP_LED_COUNT, STRIP_DATA_PIN, STRIP_CLOCK_PIN);

const String CMD_TERMINATOR = "x";
const String COLOR_CMD = "c";
const String BRIGHTNESS_CMD = "b";
CommandReader reader = CommandReader(&Serial1, CMD_TERMINATOR);

void loop(void);
void processCommand(String, String *, int);
bool validateArguments(String, int, int); 

extern "C" int main(void)
{
	Serial.begin(9600);
  Serial1.begin(9600);

  while (1) {
    loop();
    yield();

  }
}

void loop() {
  reader.read(&processCommand);
  strip.updateFrame();
}

void processCommand(String cmd, String *args, int size) {
  if (cmd == COLOR_CMD) {
    if (validateArguments("set color", 3, size)) {
      strip.setColor(args[0].toInt(), args[1].toInt(), args[2].toInt());
    }
  } else if (cmd == BRIGHTNESS_CMD) {
    if (validateArguments("set brightness", 1, size)) {
      strip.setBrightness(args[1].toInt());
    }
  } else {
    Serial.println("Unrecognized command: ");
    Serial.print(cmd);
    for (int i = 0; i < size; i++) {
      Serial.print(" ");
      Serial.print(args[i]);
    }
    Serial.println();
  }
}

bool validateArguments(String fn, int required, int given) {
  if (given < required) {
    Serial.println(fn);
    Serial.printf("Insufficient arguments. Need %d, but given %d\n", required, given);
    return false;
  }
  return true;
}
