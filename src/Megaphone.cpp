#include "Megaphone.h"

Megaphone::Megaphone(Component *components, int size) {
  lastShoutTimeMillis = 0;
  description = "";
  description += char(0x7f);
  for (int i = 0; i < size; i++) {
    description += char(components[i].getComponentType() & 0xff);
  }
  Serial.print("Megaphone");
  for (int i = 0; i < description.length(); i++) {
    Serial.println(description.charAt(i), DEC);
  }
}

void Megaphone::shout() {
  unsigned long time = millis();
  if (time - lastShoutTimeMillis < SHOUT_PERIOD_MILLIS) {
    return;
  }
  lastShoutTimeMillis = time;
  Serial.print("Description");
  Serial1.print(description);
}
