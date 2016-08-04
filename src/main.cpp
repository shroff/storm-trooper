#include "Arduino.h"
#include "Component.h"
#include "StripComponent.h"
#include "CommandReader.h"

#define CMD_TERMINATOR "x"
#define NUM_COMPONENTS 1

// Number of RGB LEDs in strand:
const int STRIP_LED_COUNT = 32;
const int STRIP_DATA_PIN  = 2;
const int STRIP_CLOCK_PIN = 3;

Component *components;

CommandReader reader = CommandReader(&Serial1, CMD_TERMINATOR);

void loop(void);
void processCommand(String, String *, int);
bool validateArguments(String, int, int); 

void setupComponents() {
  components = {
    // DO NOT forget to update NUM_COMPONENTS
    new StripComponent(STRIP_LED_COUNT, STRIP_DATA_PIN, STRIP_CLOCK_PIN)
  };
}

extern "C" int main(void)
{
	Serial.begin(9600);
  Serial1.begin(9600);

  setupComponents();

  while (1) {
    loop();
    yield();
  }
}

void loop() {
  reader.read(&processCommand);
  for (int i = 0; i < NUM_COMPONENTS; i++) {
    components[i].update();
  }
}

void processCommand(String cmd, String *args, int size) {
  Serial.println("Command Received");
  if (cmd == "command") {
    int component = args[0].toInt();
    components[component].processCommand(args[1], args + 2, size - 2);
  }
}

