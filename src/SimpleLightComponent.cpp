#include "SimpleLightComponent.h"
#include "utils.h"

#define BRIGHTNESS_CMD "b"

SimpleLightComponent::SimpleLightComponent(int pwmPin) {
  this->pwmPin = pwmPin;
  pinMode(pwmPin, OUTPUT);
}

int SimpleLightComponent::getComponentType() {
  return DIMMABLE;
}

void SimpleLightComponent::processCommand(String cmd, String *args, int size) {
  if (cmd == BRIGHTNESS_CMD) {
    if (validateArguments("brightness", 2, size)) {
      setBrightness(args[0].toInt(), args[1].toInt());
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

void SimpleLightComponent::update() {
  if (!animating) {
    return;
  }
  unsigned long currentTime = millis();

  if (currentTime >= anim_end) {
    animating = false;
    c_brightness = t_brightness;
    updateBrightness();
  } else if (currentTime > anim_next) {
    float interpolation = (currentTime - anim_start) * 1.0f / anim_duration;
    anim_next += anim_step;
    c_brightness = s_brightness + (int)((t_brightness - s_brightness) * interpolation);
    updateBrightness();
  }
}

void SimpleLightComponent::setBrightness(int brightness, int duration) {
  setupAnimation(duration);
  t_brightness = brightness & 0xff;
  Serial.printf("Setting Brightness to %d (%dms)\n", t_brightness, duration);
}

void SimpleLightComponent::setupAnimation(int duration) {
  s_brightness = c_brightness;
  anim_start = millis();
  anim_duration = duration;
  anim_next = anim_start + anim_step;
  anim_end = anim_start + anim_duration;
  animating = true;
  Serial.printf("Resetting animation: %d (%dms)\n", s_brightness, anim_duration);
}

void SimpleLightComponent::updateBrightness() {
  analogWrite(pwmPin, c_brightness);
}
