#ifndef __LINK_H__
#define __LINK_H__

#include "object.h"

// link object needs to know of the world, so it can
// pull the relevant teleporters' locations out.
class World;

class Link : public Object {
  public:
    Link();
    Link(const Link &l);
    Link operator = (const Link &l);
    virtual void copyFrom(Object *o);

    bool init();
    void render(bool transparent = false, bool selected = false, int name = -1);

    void write(ostream &stream);
    void write(char *data);
    bool read(char *data);

    bool setItemInfo(char *name, int data);
    bool getItemInfo(char *name, int &data);

  private:
    int from_side;
    int to_side;
    World *world;
    int model;
};

#endif
