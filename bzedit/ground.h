#ifndef __GROUND_H__
#define __GROUND_H__

#include "object.h"

class Ground : public Object {
  public:
    bool init();
    void render(bool transparent = false, bool selected = false, int name = -1);

  private:
    int texture;
    int model;
};

#endif
