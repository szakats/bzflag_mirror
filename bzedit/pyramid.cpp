#include "pyramid.h"
#include "util.h"
#include <stdio.h>

Pyramid::Pyramid() {
  Object::defaultData();
  px = py = pz = 0;
  sx = sy = 8.2;
  sz = 10.25;
  angle = 0;
  model = -1;
}

Pyramid::Pyramid(const Pyramid &p) {
  this->px = p.px;
  this->py = p.py;
  this->pz = p.pz;
  this->sx = p.sx;
  this->sy = p.sy;
  this->sz = p.sz;
  this->angle = p.angle;
  this->texman = p.texman;
  this->texture = p.texture;
  this->model = p.model;
}

Pyramid Pyramid::operator = (const Pyramid &p) {
  this->px = p.px;
  this->py = p.py;
  this->pz = p.pz;
  this->sx = p.sx;
  this->sy = p.sy;
  this->sz = p.sz;
  this->angle = p.angle;
  this->texman = p.texman;
  this->texture = p.texture;
  this->model = p.model;
  return *this;
}

void Pyramid::copyFrom(Object *o) {
  Pyramid *p = (Pyramid *) o;

  px = p->px;
  py = p->py;
  pz = p->pz;
  sx = p->sx;
  sy = p->sy;
  sz = p->sz;
  angle = p->angle;
  texman = p->texman;
  texture = p->texture;
  model = p->model;
}

bool Pyramid::init() {
  if(!(texman && modman))
    return false;
  texture = texman->getID("pyrwall");
  listName = "pyramid";
  if(model < 0)
    model = modman->newModel("pyramid");
  BaseModel *mod = modman->getModel(model);

  Point3D pos, scale;
  pos.x = px; pos.y = py; pos.z = pz;
  scale.x = sx; scale.y = sy; scale.z = sz;

  mod->setTexture(texture);
  mod->init(pos, scale, angle);

  return true;
}

void Pyramid::render(bool transparent, bool selected, int name) {
  if(modman)
    modman->addDrawItem(model, transparent, selected, name);
}

void Pyramid::write(ostream &stream) {
  stream << "pyramid" << endl;
  stream << "position " << px << ' ' << py << ' ' << pz << endl;
  stream << "rotation " << angle << endl;
  stream << "size " << sx << ' ' << sy << ' ' << sz << endl;
  stream << "end" << endl;
  stream << endl;
}

void Pyramid::write(char *data) {
  sprintf(data, "pyramid\nposition %f %f %f\nrotation %f\nsize %f %f %f\nend\n\n", px, py, pz, angle, sx, sy, sz);
}

bool Pyramid::read(char *data) {
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
