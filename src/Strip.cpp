#include "Strip.h"
#include "utils.h"

Strip::Strip(int ledCount, int dataPin, int clockPin) {
  strip = LPD8806(ledCount, dataPin, clockPin);
  strip.begin();
  strip.show();
}

void Strip::setColor(int r, int g, int b, int duration) {
  resetAnimation(duration);
  t_red = r & 0x7f;
  t_green = g & 0x7f;
  t_blue = b & 0x7f;
  Serial.printf("Setting Color: %d, %d, %d\n", t_red, t_green, t_blue, duration);
}

void Strip::pulse(int r1, int g1, int b1, int r2, int g2, int b2, int duration) {
  setColor(r1, g1, b1, duration);
  pulsing = true;
  p_red = r2 & 0x7f;
  p_green = g2 & 0x7f;
  p_blue = b2 & 0x7f;
  Serial.printf("Pulsing %d, %d, %d, %d, %d, %d, Duration: %d\n", r1, g1, b1, r2, g2, b2, anim_duration);
}

void Strip::resetAnimation(int duration) {
  s_red = c_red;
  s_green = c_green;
  s_blue = c_blue;
  anim_start = millis();
  anim_duration = duration;
  anim_next = anim_start + anim_step;
  anim_end = anim_start + anim_duration;
  animating = true;
  pulsing = false;
  Serial.printf("Resetting animation: %d %d %d %d\n", s_red, s_green, s_blue, anim_duration);
}

void Strip::updateFrame() {
  if (!animating) {
    return;
  }
  unsigned long currentTime = millis();
  if (pulsing && currentTime >= anim_end) {
    anim_start = anim_end;
    anim_end += anim_duration;
    s_red = t_red;
    s_green = t_green;
    s_blue = t_blue;
    swap(t_red, p_red);
    swap(t_green, p_green);
    swap(t_blue, p_blue);
  }

  if (currentTime >= anim_end) {
    animating = false;
    c_red = t_red;
    c_blue = t_blue;
    c_green = t_green;
    updateStrip();
  } else if (currentTime > anim_next) {
    float interpolation = (currentTime - anim_start) * 1.0f / anim_duration;
    anim_next += anim_step;
    c_red = s_red + (int)((t_red - s_red) * interpolation);
    c_green = s_green + (int)((t_green - s_green) * interpolation);
    c_blue = s_blue + (int)((t_blue - s_blue) * interpolation);
    updateStrip();
  }
}

void Strip::updateStrip() {
  uint32_t color = strip.Color(c_red, c_green, c_blue);
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}
