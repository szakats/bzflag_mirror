#include "walls.h"

bool Walls::init() {
  if(!(texman && modman))
    return false;

  texture = texman->getID("wall");
  listName = "walls";
  model = modman->newModel("walls");
  BaseModel *mod = modman->getModel(model);
  Point3D p;
  p.x = p.y = p.z = 0;
  mod->setTexture(texture);
  mod->init(p, p);
  return true;
}

void Walls::render(bool transparent, bool selected, int name) {
  if(modman)
    modman->addDrawItem(model, transparent, selected, name, FIRST);
}
