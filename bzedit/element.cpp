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

void Element::makeLink() {
  type = LINK;
  if(b != NULL)
    delete b;
  if(p != NULL)
    delete p;
  if(t != NULL)
    delete t;
  if(l == NULL)
    l = new Link;
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
    case LINK:
      // links are rendered directly, because
      // of their weird attributes
      return;
    case NONE:
      return;
  }
}

float Element::get_px() {
  if(type == BOX)
    return b->get_px();
  else if(type == PYRAMID)
    return p->get_px();
  else if(type == TELEPORTER)
    return t->get_px();
  else
    return 0.0;
}

float Element::get_py() {
  if(type == BOX)
    return b->get_py();
  else if(type == PYRAMID)
    return p->get_py();
  else if(type == TELEPORTER)
    return t->get_py();
  else
    return 0.0;
}

float Element::get_pz() {
  if(type == BOX)
    return b->get_pz();
  else if(type == PYRAMID)
    return p->get_pz();
  else if(type == TELEPORTER)
    return t->get_pz();
  else
    return 0.0;
}

float Element::get_angle() {
  if(type == BOX)
    return b->get_angle();
  else if(type == PYRAMID)
    return p->get_angle();
  else if(type == TELEPORTER)
    return t->get_angle();
  else
    return 0.0;
}

float Element::get_sx() {
  if(type == BOX)
    return b->get_sx();
  else if(type == PYRAMID)
    return p->get_sx();
  else if(type == TELEPORTER)
    return t->get_sx();
  else
    return 0.0;
}

float Element::get_sy() {
  if(type == BOX)
    return b->get_sy();
  else if(type == PYRAMID)
    return p->get_sy();
  else if(type == TELEPORTER)
    return t->get_sy();
  else
    return 0.0;
}

float Element::get_sz() {
  if(type == BOX)
    return b->get_sz();
  else if(type == PYRAMID)
    return p->get_sz();
  else if(type == TELEPORTER)
    return t->get_sz();
  else
    return 0.0;
}

ostream & operator << (ostream & dest, Element &src) {
  if(src.type == Element::BOX) {
    dest << (*src.b);
  } else if(src.type == Element::PYRAMID) {
    dest << (*src.p);
  } else if(src.type == Element::TELEPORTER) {
    dest << (*src.t);
  } else if(src.type == Element::LINK) {
    dest << (*src.l);
  }
  return dest;
}
