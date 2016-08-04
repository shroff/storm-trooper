#ifndef __component_h_
#define __component_h_

#include "Arduino.h"
#include "ComponentTypes.h"

class Component {
  public:
    virtual int getComponentType() = 0;
    virtual void processCommand(String cmd, String *args, int size) = 0;
    virtual void update() = 0;
};
#endif
