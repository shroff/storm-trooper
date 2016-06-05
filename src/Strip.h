#ifndef __strip_h_
#define __strip_h_

#include "LPD8806.h"

class Strip {
  public:
    Strip(int, int, int);
    void setColor(int, int, int);
    void setBrightness(int);
    void updateFrame();

  private:
    void resetAnimation();
    void updateStrip();

    LPD8806 strip;

    unsigned long anim_duration = 1000;
    unsigned long anim_step = 16;
    unsigned long anim_start = 0;
    unsigned long anim_next = 0;
    unsigned long anim_end = 0;
    bool animating = false;

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
};

#endif
