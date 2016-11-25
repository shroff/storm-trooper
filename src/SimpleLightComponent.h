#ifndef __simple_light_component_h_
#define __simple_light_component_h_

#include "Component.h"

class SimpleLightComponent : public Component {
  public:
    SimpleLightComponent(int);
    virtual int getComponentType();
    virtual void processCommand(String cmd, String *args, int size);
    virtual void update();

  private:
    int pwmPin;

    void setBrightness(int, int);
    void setupAnimation(int);
    void updateBrightness();

    unsigned long anim_duration = 0;
    unsigned long anim_step = 16;
    unsigned long anim_start = 0;
    unsigned long anim_next = 0;
    unsigned long anim_end = 0;
    bool animating = false;
    bool pulsing = false;

    int s_brightness = 0;
    int t_brightness = 0;
    int c_brightness = 0;

};

#endif
