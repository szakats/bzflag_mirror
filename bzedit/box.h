#ifndef __BOX_H__
#define __BOX_H__

#include "object.h"

class Box : public Object {
  public:
    Box();
    Box(const Box &b);
    Box operator = (const Box &b);
    virtual void copyFrom(Object *o);

    bool init();
    void render(bool transparent = false, bool selected = false, int name = -1);

    void write(ostream &stream);
    void write(char *data);
    bool read(char *data);

  private:
    int wallTexture;
    int roofTexture;
    int wallModel;
    int roofModel;
};

#endif
