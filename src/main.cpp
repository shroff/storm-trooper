#include "Arduino.h"
#include "Component.h"
#include "StripComponent.h"
#include "CommandReader.h"

// Number of RGB LEDs in strand:
const int STRIP_LED_COUNT = 32;
const int STRIP_DATA_PIN  = 2;
const int STRIP_CLOCK_PIN = 3;
Component *stripComponent = new StripComponent(STRIP_LED_COUNT, STRIP_DATA_PIN, STRIP_CLOCK_PIN);

const String CMD_TERMINATOR = "x";
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
  stripComponent->update();
}

void processCommand(String cmd, String *args, int size) {
  stripComponent->processCommand(cmd, args, size);
}

