#include "element.h"

Element::Element() {
  type = NONE;
  name = "";
  visible = true;
  selected = false;
  b = NULL;
  p = NULL;
  t = NULL;
}

Element::Element(const Element &r) {
  type = r.type;
  b = r.b;
  p = r.p;
  t = r.t;
  name = r.name;
  visible = r.visible;
  selected = r.selected;
}

Element Element::operator = (const Element &r) {
  type = r.type;
  b = r.b;
  p = r.p;
  t = r.t;
  name = r.name;
  visible = r.visible;
  selected = r.selected;
  return *this;
}

void Element::makeBox() {
  type = BOX;
  if(b == NULL)
    b = new Box;
  if(p != NULL)
    delete p;
  if(t != NULL)
    delete t;
}

void Element::makePyramid() {
  type = PYRAMID;
  if(b != NULL)
    delete b;
  if(p == NULL)
    p = new Pyramid;
  if(t != NULL)
    delete t;
}

void Element::makeTeleporter() {
  type = TELEPORTER;
  if(b != NULL)
    delete b;
  if(p != NULL)
    delete p;
  if(t == NULL)
    t = new Teleporter;
}

void Element::render(Camera &c, bool transparent) {
  switch(type) {
    case BOX:
      b->render(c, transparent);
      return;
    case PYRAMID:
      p->render(c, transparent);
      return;
    case TELEPORTER:
      t->render(c, transparent);
      return;
    case NONE:
      return;
  }
}

ostream & operator << (ostream & dest, Element &src) {
  if(src.type == Element::BOX) {
    dest << (*src.b);
  } else if(src.type == Element::PYRAMID) {
    dest << (*src.p);
  } else if(src.type == Element::TELEPORTER) {
    dest << (*src.t);
  }
  return dest;
}
