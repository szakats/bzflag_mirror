#include <stdio.h>
#include <math.h>
#include <fstream>
#include "world.h"
#include "util.h"

World::World() {
  texman = NULL;
  modman = NULL;
}

World::World(const World &w) {
  this->texman = w.texman;
  this->modman = w.modman;
  this->list = w.list;
  this->selItem = w.selItem;
}

World::~World() {
  clear();
}

World World::operator = (const World &w) {
  this->texman = w.texman;
  this->modman = w.modman;
  this->list = w.list;
  this->selItem = w.selItem;
  return *this;
}

void World::init() {
  ground.setTextureManager(texman);
  ground.setModelManager(modman);
  ground.init();

  walls.setTextureManager(texman);
  walls.setModelManager(modman);
  walls.init();
}

void World::clear() {
  ElementList::iterator it = list.begin();
  while(it != list.end()) {
    if(it->obj)
      delete(it->obj);
    it++;
  }
  list.clear();
}

void World::render(double transparency) {
  bool transparent = false;
  modman->clearDrawList();
  ElementList::iterator it;

  walls.render();
  ground.render();
  
  Element *selected = NULL;

  it = list.begin();
  while(it != list.end()) {
    if(!it->selected)
      it->render(transparent, false);
    else
      selected = &(*it);
    it++;
  }

  if(selected) {
    selected->render(transparent, true);
  }

  modman->renderList(transparency);
  modman->clearDrawList();
}

void World::render_targets() {
  ElementList::iterator it = list.begin();
  while(it != list.end()) {
    glLoadName(it->listID + 1);
    it->render(false, false, it->listID);
    it++;
  }
  modman->renderList();
  modman->clearDrawList();
}

void World::uniqueName(Element *el) {
  char name[255];
  sprintf(name, "%s-%d", el->obj->getListName(), el->listID);
  el->name = name;
}

void World::uniqueName(int item) {
  if((item >= 0) && (item < list.size()))
    uniqueName(&list[item]);
}

bool World::load(const char *filename) {
  // clear out the old map
  clear();
  return append(filename);
}

bool World::append(const char *filename) {
  if(!filename)
    return false;

  FILE *fp = fopen(filename, "rt");
  if(!fp)
    return false;

  int count = 0;
  char temp;

  while(fread(&temp, 1, 1, fp) != 0)
    count++;

  fseek(fp, 0, SEEK_SET);

  char *data = (char *) malloc(count + 1);

  if(!data) {
    fclose(fp);
    return false;
  }

  fread(data, count, 1, fp);
  data[count] = '\0';
  fclose(fp);

  // now we have the file

  char *ptr = data, element[1024], name[255];
  bool done = false;

  Element item;

  while(!done) {
    done = getNextElement(&ptr, element);
    if(element[0]) {
      sscanf(element, "%s", name);
      item.init();

      if(stricmp(name, "BOX") == 0) {
	item.makeBox();
      } else if(stricmp(name, "PYRAMID") == 0) {
	item.makePyramid();
      } else if(stricmp(name, "TELEPORTER") == 0) {
	item.makeTeleporter();
      } else if(stricmp(name, "LINK") == 0) {
	item.makeLink();
	item.obj->setItemInfo("World", (int) this);
      } else if(stricmp(name, "BASE") == 0) {
	item.makeBase();
      } else {
	clear();
	return false;
	  }

      if(item.obj) {
	item.obj->setTextureManager(texman);
	item.obj->setModelManager(modman);
	item.obj->read(element);
	item.obj->init();

	item.listID = list.size();
	uniqueName(&item);

	list.push_back(item);

	item.obj = NULL;
      }
    }
  }
  free(data);
  return true;
}

bool World::write(const char *filename) {
  fstream file;

  file.open(filename, ios::out);
  if(!file)
    return false;

  file << "# World built with bzedit, available at\n";
  file << "# http://bzedit.bolognia.net/\n\n";

  for(unsigned int i = 0; i < list.size(); i++) {
    file << "#!name " << list[i].name.c_str() << endl;
    list[i].obj->write(file);
  }
  file.close();
  return true;
}

ObjectType World::getElementType(Element *el) {
  return el->type;
}

ObjectType World::getElementType(int item) {
  if((item >= 0) && (item < list.size()))
    return getElementType(&list[item]);
  return NONE;
}

Element *World::getElement(char *name) {
  ElementList::iterator it = list.begin();
  while(it != list.end()) {
    if(stricmp(it->name.c_str(), name) == 0)
      return &(*it);
    it++;
  }
  return NULL;
}

Element *World::getElement(int item) {
  if((item >= 0) && (item < list.size()))
    return &(list[item]);
  return NULL;
}

Element *World::getNthElementOfType(ObjectType type, int n) {
  int count = 0;
  ElementList::iterator it = list.begin();
  while(it != list.end()) {
    if(type == it->type) {
      if(count == n)
	return &(*it);
      count++;
    }
    it++;
  }
  return NULL;
}

int World::addElement(ObjectType type) {
  Element item;
  item.init();

  ElementList::iterator it = list.begin();
  while(it != list.end()) {
    it->selected = false;
    it++;
  }

  switch(type) {
    case BOX:
      item.makeBox();
      break;
    case PYRAMID:
      item.makePyramid();
      break;
    case TELEPORTER:
      item.makeTeleporter();
      break;
    case LINK:
      item.makeLink();
      item.obj->setItemInfo("World", (int)this);
      break;
    case BASE:
      item.makeBase();
      break;
    default:
      // what kind of weird object is this?
      return -1;
  }
  item.selected = true;
  item.obj->setTextureManager(texman);
  item.obj->setModelManager(modman);
  item.obj->init();

  item.listID = list.size();
  uniqueName(&item);

  list.push_back(item);
  return item.listID;
}

bool World::removeElement(int item) {
  ElementList::iterator it = list.begin();
  bool gotone = false;

  while(it != list.end()) {
    if(it->listID == item) {
      it = list.erase(it);
      gotone = true;
    } else {
      it++;
    }
  }
  return gotone;
}

int World::findElementWithName(std::string name) {
  for(int i = 0; i < list.size(); i++) {
    if(list[i].name == name)
      return i;
  }
  return -1;
}

int World::getNumOfType(ObjectType type) {
  int retval = 0;
  ElementList::iterator it = list.begin();

  while(it != list.end()) {
    if(it->type == type)
      retval++;
    it++;
  }
  return retval;
}

void World::sort() {
  ElementList newlist;
  ElementList::iterator it;

  for(int type = BOX; type < NONE; type++) {
    it = list.begin();
    while(it != list.end()) {
      if(it->type == type)
	newlist.push_back(*it);
      it++;
    }
  }
  list.clear();
  list = newlist;
}

void World::setTextureManager(TextureManager *texman) {
  this->texman = texman;
  ground.setTextureManager(texman);
  ground.init();
  walls.setTextureManager(texman);
  walls.init();
}

void World::setModelManager(ModelManager *modman) {
  this->modman = modman;
  ground.setModelManager(modman);
  ground.init();
  walls.setModelManager(modman);
  walls.init();
}
