#include "element.h"

Element::Element() {
  type = NONE;
  name = "";
  visible = true;
  selected = false;
  b = NULL;
  p = NULL;
  t = NULL;
  l = NULL;
}

Element::Element(const Element &r) {
  type = r.type;
  b = r.b;
  p = r.p;
  t = r.t;
  l = r.l;
  name = r.name;
  visible = r.visible;
  selected = r.selected;
}

Element Element::operator = (const Element &r) {
  type = r.type;
  b = r.b;
  p = r.p;
  t = r.t;
  l = r.l;
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
  if(l != NULL)
    delete l;
}

void Element::makePyramid() {
  type = PYRAMID;
  if(b != NULL)
    delete b;
  if(p == NULL)
    p = new Pyramid;
  if(t != NULL)
    delete t;
  if(l != NULL)
    delete l;
}

void Element::makeTeleporter() {
  type = TELEPORTER;
  if(b != NULL)
    delete b;
  if(p != NULL)
    delete p;
  if(t == NULL)
    t = new Teleporter;
  if(l != NULL)
    delete l;
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
//    case LINK:
//      return;
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
//  } else if(src.type == Element::LINK) {
//    dest << (*src.l);
  }
  return dest;
}
