#ifndef __WALLS_H__
#define __WALLS_H__

#include "object.h"

class Walls : public Object {
  public:
    bool init();
    void render(bool transparent = false, bool selected = false, int name = -1);

  private:
    int texture;
    int model;
};

#endif
