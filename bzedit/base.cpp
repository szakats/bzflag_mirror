#include "base.h"
#include "util.h"
#include <stdio.h>

Base::Base() {
  Object::defaultData();
  listName = "base";

  color = 1;
  sx = 30;
  sy = 30;
  sz = 0;

  baseModel = -1;
  wallModel = -1;
}

Base::Base(const Base &b) {
  this->px = b.px;
  this->py = b.py;
  this->pz = b.pz;
  this->sx = b.sx;
  this->sy = b.sy;
  this->sz = b.sz;
  this->angle = b.angle;
  this->color = b.color;
  this->baseModel = b.baseModel;
  this->wallModel = b.wallModel;
}

Base Base::operator = (const Base &b) {
  this->px = b.px;
  this->py = b.py;
  this->pz = b.pz;
  this->sx = b.sx;
  this->sy = b.sy;
  this->sz = b.sz;
  this->angle = b.angle;
  this->color = b.color;
  this->baseModel = b.baseModel;
  this->wallModel = b.wallModel;
  return *this;
}

void Base::copyFrom(Object *o) {
  Base *b = (Base *) o;

  px = b->px;
  py = b->py;
  pz = b->pz;
  sx = b->sx;
  sy = b->sy;
  sz = b->sz;
  angle = b->angle;
  color = b->color;
  baseModel = b->baseModel;
  wallModel = b->wallModel;
}

bool Base::init() {
  if(!(texman && modman))
    return false;
  int wallTexture = texman->getID("boxwall");
  listName = "base";

  if(baseModel < 0)
    baseModel = modman->newModel("basetops");
  BaseModel *mod = modman->getModel(baseModel);
  if(!mod)
    return false;

  Point3D pos, scale;
  pos.x = px; pos.y = py; pos.z = pz;
  scale.x = sx; scale.y = sy; scale.z = sz;

  mod->setTexture(-1);
  float temp = color;
  mod->setItemInfo("color", &temp);
  mod->init(pos, scale, angle);

  if(wallModel < 0) {
    if(pos.z > 0)
      wallModel = modman->newModel("boxsides");
  } else {
    if(pos.z == 0) {
      modman->killModel(wallModel);
      wallModel = -1;
    }
  }

  if(wallModel != -1) {
    mod = modman->getModel(wallModel);
    if(!mod)
      return false;
    scale.z = 1.0;
    mod->setTexture(wallTexture);
    mod->init(pos, scale, angle);
  }
  return true;
}

void Base::render(bool transparent, bool selected, int name) {
  if(modman) {
    modman->addDrawItem(baseModel, transparent, selected, name, ALWAYS);
    if(wallModel != -1)
      modman->addDrawItem(wallModel, transparent, selected, name, HIGH);
  }
}

void Base::write(ostream &stream) {
  stream << "base" << endl;
  stream << "position " << px << ' ' << py << ' ' << pz << endl;
  stream << "rotation " << angle << endl;
  stream << "size " << sx << ' ' << sy << ' ' << sz << endl;
  stream << "color " << color << endl;
  stream << "end" << endl;
  stream << endl;
}

void Base::write(char *data) {
  sprintf(data, "base\nposition %f %f %f\nrotation %f\nsize %f %f %f\ncolor %i\nend\n\n", px, py, pz, angle, sx, sy, sz, color);
}

bool Base::read(char *data) {
  char line[255];
  char *ptr = data, *dataStart;
  char name[255];

  bool done = false;

  while(!done) {
    done = !getLine(&ptr, line);

    if(!stdDataField(line)) {
      sscanf(line, "%s", name);
      dataStart = line + strlen(name);

      if(stricmp(name, "end") == 0) {
	done = true;
      } else if(stricmp(name, "color") == 0) {
	color = atoi(dataStart);
      }
    }
  }
  return true;
}

bool Base::setItemInfo(char *name, int data) {
  if(stricmp(name, "color") == 0) {
    color = data;
    return true;
  }
  return false;
}

bool Base::getItemInfo(char *name, int &data) {
  if(stricmp(name, "color") == 0) {
    data = color;
    return true;
  }
  return false;
}
