#include "teleporter.h"
#include "util.h"
#include <stdio.h>

Teleporter::Teleporter() {
  Object::defaultData();
  px = py = pz = 0;
  sx = 0;
  sy = 4.48;
  sz = 20.16;
  angle = 0;
  border = 1;
  fieldModel = -1;
  frameModel = -1;
}

Teleporter::Teleporter(const Teleporter &t) {
  this->px = t.px;
  this->py = t.py;
  this->pz = t.pz;
  this->sx = t.sx;
  this->sy = t.sy;
  this->sz = t.sz;
  this->angle = t.angle;
  this->texman = t.texman;
  this->texture = t.texture;
  this->fieldModel = t.fieldModel;
  this->frameModel = t.frameModel;
}

Teleporter Teleporter::operator = (const Teleporter &t) {
  this->px = t.px;
  this->py = t.py;
  this->pz = t.pz;
  this->sx = t.sx;
  this->sy = t.sy;
  this->sz = t.sz;
  this->angle = t.angle;
  this->texman = t.texman;
  this->texture = t.texture;
  this->fieldModel = t.fieldModel;
  this->frameModel = t.frameModel;
  return *this;
}

void Teleporter::copyFrom(Object *o) {
  Teleporter *t = (Teleporter *) o;

  px = t->px;
  py = t->py;
  pz = t->pz;
  sx = t->sx;
  sy = t->sy;
  sz = t->sz;
  angle = t->angle;
  texman = t->texman;
  texture = t->texture;
  fieldModel = t->fieldModel;
  frameModel = t->frameModel;
}

bool Teleporter::init() {
  if(!(texman && modman))
    return false;
  texture = texman->getID("caution");
  listName = "teleporter";

  if(fieldModel < 0)
    fieldModel = modman->newModel("tportfield");
  BaseModel *mod = modman->getModel(fieldModel);
  if(!mod)
    return false;

  Point3D pos, scale;
  pos.x = px; pos.y = py; pos.z = pz;
  scale.x = sx; scale.y = sy; scale.z = sz;
  mod->setTexture(-1);
  mod->init(pos, scale, angle);

  if(frameModel < 0)
    frameModel = modman->newModel("tportframe");
  mod = modman->getModel(frameModel);
  if(!mod)
    return false;

  mod->setItemInfo("border", &border);
  mod->setTexture(texture);
  mod->init(pos, scale, angle);

  return true;
}

void Teleporter::render(bool transparent, bool selected, int name) {
  if(modman) {
    modman->addDrawItem(fieldModel, transparent, selected, name, TRANSPARENT);
    modman->addDrawItem(frameModel, transparent, selected, name);
  }
}

void Teleporter::write(ostream &stream) {
  stream << "teleporter" << endl;
  stream << "position " << px << ' ' << py << ' ' << pz << endl;
  stream << "rotation " << angle << endl;
  stream << "size " << sx << ' ' << sy << ' ' << sz << endl;
  stream << "border " << border << endl;
  stream << "end" << endl;
  stream << endl;
}

void Teleporter::write(char *data) {
  sprintf(data, "teleporter\nposition %f %f %f\nrotation %f\nsize %f %f %f\nborder %f\nend\n\n", px, py, pz, angle, sx, sy, sz, border);
}

bool Teleporter::read(char *data) {
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
      } else if(stricmp(name, "border") == 0) {
	border = (float) atof(dataStart);
      }
    }
  }
  return true;
}

bool Teleporter::setItemInfo(char *name, float data) {
  if(stricmp(name, "border") == 0) {
    border = data;
    return true;
  }
  return false;
}

bool Teleporter::getItemInfo(char *name, float &data) {
  if(stricmp(name, "border") == 0) {
    data = border;
    return true;
  }
  return false;
}
