#include "Arduino.h"
#include "LPD8806.h"
#include "SPI.h"

// Number of RGB LEDs in strand:
const int STRIP_LED_COUNT = 32;
const int STRIP_DATA_PIN  = 2;
const int STRIP_CLOCK_PIN = 3;
LPD8806 strip = LPD8806(STRIP_LED_COUNT, STRIP_DATA_PIN, STRIP_CLOCK_PIN);

const String COLOR_CMD = "c";
const String BRIGHTNESS_CMD = "b";

String command = "";
const int CMD_SIZE = 16;
String cmd[CMD_SIZE];
int cmdIndex;

unsigned long anim_duration = 1000;
unsigned long anim_step = 16;
unsigned long anim_start = 0;
unsigned long anim_next = 0;
unsigned long anim_end = 0;
boolean animating = false;

int s_red = 0;
int s_green = 0;
int s_blue = 0;
int s_brightness = 0;
int t_red = 0;
int t_green = 0;
int t_blue = 0;
int t_brightness = 0;
int c_red = 0;
int c_green = 0;
int c_blue = 0;
int c_brightness = 0;

void loop(void);
void updateStrip(void);
void processCommand(void);
bool validateArguments(int, String); 
void animate(void);
void setStripColor(int, int, int);
void setStripBrightness(int);
void resetAnimation(void);

extern "C" int main(void)
{
	Serial.begin(9600);
  Serial1.begin(9600);
  strip.begin();
  strip.show();

  while (1) {
    loop();
    yield();

  }
}

void loop() {
  if (Serial1.available() > 0) {
    char c = Serial1.read();
    Serial.print(c);
    if (c == ' ' || c == '\n' || c == '\r') {
      Serial.print("sep");
      if (command.length() > 0) {
        if (command == "x") {
          processCommand();
        } else if (cmdIndex < CMD_SIZE) {
          cmd[cmdIndex++] = command;
        } else {
          Serial.print("Dropped command: ");
          Serial.println(command);
        }
        command = "";
      }
    } else if (c != '\r') {
      // Ignore CR
      command += c;
    }
  }
  animate();
}

void processCommand() {
  if (cmdIndex > 0) {
    if (cmd[0] == COLOR_CMD) {
      if (validateArguments(3, "set color")) {
        setStripColor(cmd[1].toInt(), cmd[2].toInt(), cmd[3].toInt());
      }
    } else if (cmd[0] == BRIGHTNESS_CMD) {
      if (validateArguments(1, "set brightness")) {
        setStripBrightness(cmd[1].toInt());
      }
    } else {
      Serial.println("Unrecognized command: ");
      Serial.print(cmd[0]);
      for (int i = 1; i < cmdIndex; i++) {
        Serial.print(" ");
        Serial.print(cmd[i]);
      }
      Serial.println();
    }
  }
  cmdIndex = 0;
}

boolean validateArguments(int argCount, String fn) {
  if (cmdIndex < argCount + 1) {
    Serial.println(fn);
    Serial.printf("Insufficient arguments. Need %d, but given %d\n", argCount, cmdIndex - 1);
    return false;
  }
  return true;
}

void setStripColor(int r, int g, int b) {
  resetAnimation();
  t_red = r & 0x7f;
  t_green = g & 0x7f;
  t_blue = b & 0x7f;
  t_brightness = 0x80;
  Serial.printf("Setting Color: %d, %d, %d\n", t_red, t_green, t_blue);
}

void setStripBrightness(int b) {
  resetAnimation();
  t_brightness = (b & 0x7f) + 1;
  Serial.printf("Setting Brightness: %d\n", t_brightness);
}

void resetAnimation() {
  s_red = c_red;
  s_green = c_green;
  s_blue = c_blue;
  s_brightness = c_brightness;
  anim_start = millis();
  anim_next = anim_start + anim_step;
  anim_end = anim_start + anim_duration;
  animating = true;
}

void animate() {
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

void updateStrip() {
  int r = (c_red * c_brightness) >> 7;
  int g = (c_green * c_brightness) >> 7;
  int b = (c_blue * c_brightness) >> 7;
  uint32_t color = strip.Color(r, g, b);
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

