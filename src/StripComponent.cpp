#include "StripComponent.h"
#include "utils.h"

#define COLOR_CMD "c"
#define PULSE_CMD "p"

StripComponent::StripComponent(int ledCount, int dataPin, int clockPin) {
  strip = new Strip(ledCount, dataPin, clockPin);
}

void StripComponent::processCommand(String cmd, String *args, int size) {
  if (cmd == COLOR_CMD) {
    if (validateArguments("set color", 4, size)) {
      strip->setColor(args[0].toInt(), args[1].toInt(), args[2].toInt(), args[3].toInt());
    }
  } else if (cmd == PULSE_CMD) {
    if (validateArguments("pulse", 7, size)) {
      strip->pulse(args[0].toInt(), args[1].toInt(), args[2].toInt(), args[3].toInt(), args[4].toInt(), args[5].toInt(), args[6].toInt());
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

void StripComponent::update() {
  strip->updateFrame();
}

int StripComponent::getComponentType() {
  return RGB_LED;
}
