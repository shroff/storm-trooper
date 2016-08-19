#include "Megaphone.h"

Megaphone::Megaphone(Component *components, int size) {
  lastShoutTimeMillis = 0;
  for (int i = 0; i < size; i++) {
    if (i != 0) {
      description += " ";
    }
    description += String(components[i].getComponentType());
  }
}

void Megaphone::shout() {
  unsigned long time = millis();
  if (time - lastShoutTimeMillis < SHOUT_PERIOD_MILLIS) {
    return;
  }
  lastShoutTimeMillis = time;
  Serial.print("Description: ");
  Serial.println(description);
  Serial1.print(description);
}
