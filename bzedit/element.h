#ifndef __ELEMENT_H__
#define __ELEMENT_H__

#include <string>
#include "object.h"

typedef enum {
  BOX = 0,
  PYRAMID,
  TELEPORTER,
  LINK,
  BASE,
  NONE
} ObjectType;

class Element {
  public:
    Element();
    Element(const Element &e);
    Element operator = (const Element &e);

    void init();
    void makeBox();
    void makePyramid();
    void makeTeleporter();
    void makeLink();
    void makeBase();

    void render(bool transparent, bool selected, int name = -1);

    ObjectType type;
    Object *obj;

    std::string name;
    bool visible;
    bool selected;

    int listID;
};

#endif
