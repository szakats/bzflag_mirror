#ifndef __ELEMENT_H__
#define __ELEMENT_H__

#include <string>
#include "objects.h"

class Element {
  public:
//    enum {BOX, PYRAMID, TELEPORTER, LINK, NONE} type;
    enum {BOX, PYRAMID, TELEPORTER, NONE} type;
    Box *b;
    Pyramid *p;
    Teleporter *t;
    Link *l;
    string name;
    bool visible;
    bool selected;

    Element();
    Element(const Element &r);
    Element operator = (const Element &r);
    void makeBox();
    void makePyramid();
    void makeTeleporter();
    void render(Camera &c, bool transparent);
    friend ostream & operator << (ostream & dest, Element &src);
};

#endif
