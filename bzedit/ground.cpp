#include "ground.h"

bool Ground::init() {
  if(!(texman && modman))
    return false;

  texture = texman->getID("ground");
  listName = "ground";
  model = modman->newModel("ground");
  BaseModel *mod = modman->getModel(model);
  Point3D p;
  p.x = p.y = p.z = 0;
  mod->setTexture(texture);
  mod->init(p, p);
  return true;
}

void Ground::render(bool transparent, bool selected, int name) {
  if(modman)
    modman->addDrawItem(model, transparent, selected, name, FIRST);
}
