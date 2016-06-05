#include "Strip.h"

Strip::Strip(int ledCount, int dataPin, int clockPin) {
  strip = LPD8806(ledCount, dataPin, clockPin);
  strip.begin();
  strip.show();
}


void Strip::setColor(int r, int g, int b) {
  resetAnimation();
  t_red = r & 0x7f;
  t_green = g & 0x7f;
  t_blue = b & 0x7f;
  t_brightness = 0x80;
  //Serial.printf("Setting Color: %d, %d, %d\n", t_red, t_green, t_blue);
}

void Strip::setBrightness(int b) {
  resetAnimation();
  t_brightness = (b & 0x7f) + 1;
  //Serial.printf("Setting Brightness: %d\n", t_brightness);
}

void Strip::resetAnimation() {
  s_red = c_red;
  s_green = c_green;
  s_blue = c_blue;
  s_brightness = c_brightness;
  anim_start = millis();
  anim_next = anim_start + anim_step;
  anim_end = anim_start + anim_duration;
  animating = true;
}

void Strip::updateFrame() {
  if (!animating) {
    return;
  }
  unsigned long currentTime = millis();
  if (currentTime >= anim_end) {
    animating = false;
    c_red = t_red;
    c_blue = t_blue;
    c_green = t_green;
    c_brightness = t_brightness;
    updateStrip();
  } else if (currentTime > anim_next) {
    float interpolation = (currentTime - anim_start) * 1.0f / anim_duration;
    anim_next += anim_step;
    c_red = s_red + (int)((t_red - s_red) * interpolation);
    c_green = s_green + (int)((t_green - s_green) * interpolation);
    c_blue = s_blue + (int)((t_blue - s_blue) * interpolation);
    c_brightness = s_brightness + (int)((t_brightness - s_brightness) * interpolation);
    updateStrip();
  }
}

void Strip::updateStrip() {
  int r = (c_red * c_brightness) >> 7;
  int g = (c_green * c_brightness) >> 7;
  int b = (c_blue * c_brightness) >> 7;
  uint32_t color = strip.Color(r, g, b);
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}
