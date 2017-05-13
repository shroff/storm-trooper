#include "Megaphone.h"

Megaphone::Megaphone(Stormio *stormio, Component *components, char size) {
  this->stormio = stormio;
  this->lastShoutTimeMillis = 0;
  this->description = "";

  //TODO: maybe throw an error here if the size is > 255.
  size = size & 0xff;

  for (int i = 0; i < size; i++) {
    description += char(components[i].getComponentType() & 0xff);
  }

  for (unsigned int i = 0; i < description.length(); i++) {
    Serial.println(description.charAt(i), DEC);
  }
}

void Megaphone::shout() {
  unsigned long time = millis();
  if (time - lastShoutTimeMillis < SHOUT_PERIOD_MILLIS) {
    return;
  }
  lastShoutTimeMillis = time;
  stormio->write(PACKET_TYPE_CAPABILITY, description);
}
