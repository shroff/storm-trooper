#include "Arduino.h"
#include "Component.h"
#include "StripComponent.h"
#include "CommandReader.h"
#include "utils.h"

#define CMD_TERMINATOR "x"
#define NUM_COMPONENTS 1

Component *components;
void setupComponents() {
  components = {
    // DO NOT forget to update NUM_COMPONENTS
    new StripComponent(32/*ledCount*/, 2/*dataPin*/, 3/*clockPin*/)
  };
}

CommandReader reader = CommandReader(&Serial1, CMD_TERMINATOR);

void loop(void);
void processCommand(String, String *, int);
bool validateArguments(String, int, int); 

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
  if (!validateArguments("process command", 2, size)) {
    return;
  }
  Serial.println("Command Received");
  if (cmd == "command") {
    int component = args[0].toInt();
    components[component].processCommand(args[1], args + 2, size - 2);
  }
}

