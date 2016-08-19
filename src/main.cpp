#include "Arduino.h"
#include "Component.h"
#include "StripComponent.h"
#include "CommandReader.h"
#include "Megaphone.h"
#include "utils.h"


Component* getComponents() {
  return {
#define NUM_COMPONENTS 1
    new StripComponent(32/*ledCount*/, 2/*dataPin*/, 3/*clockPin*/)
  };
}

Component *components;
Megaphone *megaphone;
CommandReader *reader;

void loop(void);
void processCommand(String, String *, int);
bool validateArguments(String, int, int); 

extern "C" int main(void)
{
	Serial.begin(9600);
  Serial1.begin(9600);

  components = getComponents();
  megaphone = new Megaphone(components, NUM_COMPONENTS);
  reader = new CommandReader(&Serial1, "x");

  while (1) {
    loop();
    yield();
  }
}

void loop() {
  reader->read(&processCommand);
  megaphone->shout();
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

