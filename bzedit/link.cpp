#include "link.h"
#include "element.h"
#include "util.h"
#include "world.h"
#include <stdio.h>

Link::Link() {
  Object::defaultData();
  world = NULL;
  from_side = -1;
  to_side = -1;
  model = -1;
  listName = "link";
}

Link::Link(const Link &l) {
  from_side = l.from_side;
  to_side = l.to_side;
  model = l.model;
}

Link Link::operator = (const Link &l) {
  from_side = l.from_side;
  to_side = l.to_side;
  model = l.model;
  return *this;
}

void Link::copyFrom(Object *o) {
  Link *l = (Link *) o;
  from_side = l->from_side;
  to_side = l->to_side;
  model = l->model;
}

bool Link::init() {
  if(!(texman && modman))
    return false;

  int texture = texman->getID("links");
  listName = "link";
  if(model < 0)
    model = modman->newModel("link");

  BaseModel *mod = modman->getModel(model);

  if(!mod)
    return false;

  mod->setTexture(texture);
  mod->setItemInfo("from", (float *) &from_side);
  mod->setItemInfo("to", (float *) &to_side);
  mod->setItemInfo("world", (float *) world);
  
  return true;
}

void Link::render(bool transparent, bool selected, int name) {
  if(modman)
    modman->addDrawItem(model, transparent, selected, name);
}

void Link::write(ostream &stream) {
  stream << "link" << endl;
  stream << "from " << from_side << endl;
  stream << "to " << to_side << endl;
  stream << "end" << endl;
  stream << endl;
}

void Link::write(char *data) {
  sprintf(data, "link\nfrom %i\nto %i\nend\n\n", from_side, to_side);
}

bool Link::read(char *data) {
  char line[255];
  char *ptr = data, *dataStart;
  char name[255];

  bool done = false;

  while(!done) {
    done = !getLine(&ptr, line);
    sscanf(line, "%s", name);
    dataStart = line + strlen(name);

    if(stricmp(name, "end") == 0) {
      done = true;
    } else if(stricmp(name, "from") == 0) {
      from_side = atoi(dataStart);
    } else if(stricmp(name, "to") == 0) {
      to_side = atoi(dataStart);
    }
  }
  return true;
}

bool Link::setItemInfo(char *name, int data) {
  if(stricmp(name, "world") == 0) {
    world = (World *) data;
    return true;
  } else if(stricmp(name, "fromside") == 0) {
    from_side = data;
    return true;
  } else if(stricmp(name, "toside") == 0) {
    to_side = data;
    return true;
  }
  return false;
}

bool Link::getItemInfo(char *name, int &data) {
  if(stricmp(name, "fromside") == 0) {
    data = from_side;
    return true;
  } else if(stricmp(name, "toside") == 0) {
    data = to_side; return true;
  }
  return false;
}
