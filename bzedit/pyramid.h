#ifndef __PYRAMID_H__
#define __PYRAMID_H__

#include "object.h"

class Pyramid : public Object {
  public:
    Pyramid();
    Pyramid(const Pyramid &p);
    Pyramid operator = (const Pyramid &p);
    virtual void copyFrom(Object *o);

    bool init();
    void render(bool transparent = false, bool selected = false, int name = -1);

    void write(ostream &stream);
    void write(char *data);
    bool read(char *data);

  private:
    int texture;
    int model;
};

#endif
