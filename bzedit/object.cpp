#include "util.h"
#include "object.h"
#include "world.h"
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <string.h>

Object::Object() {
  defaultData();
}

Object::Object(const Object &o) {
  init();
  (*this) = o;
}

Object::~Object() {
}

void Object::copyFrom(Object *o) {
}

void Object::set_px(float px) {
  this->px = px;
}

void Object::set_py(float py) {
  this->py = py;
}

void Object::set_pz(float pz) {
  this->pz = pz;
}
  
void Object::set_sx(float sx) {
  this->sx = sx;
}

void Object::set_sy(float sy) {
  this->sy = sy;
}

void Object::set_sz(float sz) {
  this->sz = sz;
}

void Object::set_angle(float angle) {
  this->angle = angle;
}

void Object::setTextureManager(TextureManager *man) {
  this->texman = man;
}

void Object::setModelManager(ModelManager *man) {
  this->modman = man;
}

float Object::get_px() {
  return px;
}

float Object::get_py() {
  return py;
}

float Object::get_pz() {
  return pz;
}

float Object::get_sx() {
  return sx;
}

float Object::get_sy() {
  return sy;
}

float Object::get_sz() {
  return sz;
}

float Object::get_angle() {
  return angle;
}

void Object::defaultData() {
  texman = NULL;
  px = py = pz = 0;
  sx = sy = sz = 0;
  angle = 0;
  listName = "unknown";
}

void Object::render(bool transparent, bool selected, int name) {
  // just stubbed
}

void Object::write(ostream &stream) {
  // just stubbed
}

void Object::write(char *data) {
  // just stubbed
}

bool Object::read(char *data) {
  return false;
}

bool Object::init() {
  return false;
}

bool Object::setItemInfo(char *name, int data) {
  return false;
}

bool Object::setItemInfo(char *name, float data) {
  return false;
}

bool Object::setItemInfo(char *name, char *data) {
  return false;
}

bool Object::getItemInfo(char *name, int &data) {
  return false;
}

bool Object::getItemInfo(char *name, float &data) {
  return false;
}

bool Object::getItemInfo(char *name, char *data) {
  return false;
}

const char *Object::getListName() {
  return listName.c_str();
}

bool Object::stdDataField(char *line) {
  char name[255], *nameEnd;

  sscanf(line, "%s", name);
  nameEnd = line + strlen(name);

  if(stricmp(name, "position") == 0) {
    sscanf(nameEnd, " %f %f %f", &px, &py, &pz);
  } else if(stricmp(name, "rotation") == 0) {
    sscanf(nameEnd, " %f", &angle);
  } else if(stricmp(name, "size") == 0) {
    sscanf(nameEnd, " %f %f %f", &sx, &sy, &sz);
  } else {
    return false;
  }
  return true;
}
