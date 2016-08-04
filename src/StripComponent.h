#ifndef __strip_component_h_
#define __strip_component_h_

#include "Component.h"
#include "Strip.h"

class StripComponent : public Component {
  private:
    Strip *strip;

  public:
    StripComponent(int, int, int);
    virtual int getComponentType();
    virtual void processCommand(String cmd, String *args, int size);
    virtual void update();


};

#endif
