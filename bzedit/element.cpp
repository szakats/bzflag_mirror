#include "element.h"
#include "box.h"
#include "pyramid.h"
#include "teleporter.h"
#include "link.h"
#include "base.h"

Element::Element() {
  init();
  type = NONE;
  listID = 0;
}

Element::Element(const Element &e) {
  this->type = e.type;
  this->obj = e.obj;
  this->listID = e.listID;
  this->name = e.name;
  this->visible = e.visible;
  this->selected = e.selected;
}

Element Element::operator = (const Element &e) {
  this->type = e.type;
  this->obj = e.obj;
  this->listID = e.listID;
  this->name = e.name;
  this->visible = e.visible;
  this->selected = e.selected;
  return *this;
}

void Element::init() {
  name = "";
  visible = true;
  selected = false;
}

void Element::makeBox() {
  type = BOX;
  obj = new Box;
}

void Element::makePyramid() {
  type = PYRAMID;
  obj = new Pyramid;
}

void Element::makeTeleporter() {
  type = TELEPORTER;
  obj = new Teleporter;
}

void Element::makeLink() {
  type = LINK;
  obj = new Link;
}

void Element::makeBase() {
  type = BASE;
  obj = new Base;
}

void Element::render(bool transparent, bool selected, int name) {
  obj->render(transparent, selected, name);
}
