#include "box.h"
#include "util.h"
#include <stdio.h>

Box::Box() {
  Object::defaultData();
  px = py = pz = 0;
  sx = sy = 30;
  sz = 9.42;
  angle = 0;
  wallModel = -1;
  roofModel = -1;
}

Box::Box(const Box &b) {
  Object::defaultData();
  this->px = b.px;
  this->py = b.py;
  this->pz = b.pz;
  this->sx = b.sx;
  this->sy = b.sy;
  this->sz = b.sz;
  this->angle = b.angle;
  this->texman = b.texman;
  this->wallTexture = b.wallTexture;
  this->roofTexture = b.roofTexture;
  this->wallModel = b.wallModel;
  this->roofModel = b.roofModel;
}

Box Box::operator = (const Box &b) {
  this->px = b.px;
  this->py = b.py;
  this->pz = b.pz;
  this->sx = b.sx;
  this->sy = b.sy;
  this->sz = b.sz;
  this->angle = b.angle;
  this->texman = b.texman;
  this->wallTexture = b.wallTexture;
  this->roofTexture = b.roofTexture;
  this->wallModel = b.wallModel;
  this->roofModel = b.roofModel;

  return *this;
}

void Box::copyFrom(Object *o) {
  Box *box = (Box *) o;

  px = box->px;
  py = box->py;
  pz = box->pz;
  sx = box->sx;
  sy = box->sy;
  sz = box->sz;
  angle = box->angle;
  texman = box->texman;
  wallTexture = box->wallTexture;
  roofTexture = box->roofTexture;
  wallModel = box->wallModel;
  roofModel = box->roofModel;
}

bool Box::init() {
  if(!(modman && texman))
    return false;
  wallTexture = texman->getID("boxwall");
  roofTexture = texman->getID("roof");
  listName = "box";

  if(wallModel < 0)
    wallModel = modman->newModel("boxsides");
  BaseModel *mod = modman->getModel(wallModel);

  Point3D pos, scale;
  pos.x = px; pos.y = py; pos.z = pz;
  scale.x = sx; scale.y = sy; scale.z = sz;

  mod->setTexture(wallTexture);
  mod->init(pos, scale, angle);

  if(roofModel < 0)
    roofModel = modman->newModel("boxtops");
  mod = modman->getModel(roofModel);

  mod->setTexture(roofTexture);
  mod->init(pos, scale, angle);

  return true;
}

void Box::render(bool transparent, bool selected, int name) {
  if(modman) {
    modman->addDrawItem(wallModel, transparent, selected, name);
    modman->addDrawItem(roofModel, transparent, selected, name);
  }
}

void Box::write(ostream &stream) {
  stream << "box" << endl;
  stream << "position " << px << ' ' << py << ' ' << pz << endl;
  stream << "rotation " << angle << endl;
  stream << "size " << sx << ' ' << sy << ' ' << sz << endl;
  stream << "end" << endl;
  stream << endl;
}

void Box::write(char *data) {
  sprintf(data, "box\nposition %f %f %f\nrotation %f\nsize %f %f %f\nend\n\n", px, py, pz, angle, sx, sy, sz);
}

bool Box::read(char *data) {
  char line[255];
  char *ptr = data;
  char name[255];

  bool done = false;

  while(!done) {
    done = !getLine(&ptr, line);
    if(!stdDataField(line)) {
      sscanf(line, "%s", name);

      if(stricmp(name, "end") == 0)
	done = true;
    }
  }
  return true;
}
