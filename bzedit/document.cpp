#include "document.h"
#include "globals.h"
#include <iostream>

Document::Document(const string& f) {
  open = true;
  hasname = false;
  unmodified = true;
  listItem = -1;
  lastaction = false;
  nextaction = true;
  saveaction = NULL;
  exiting = false;
  texman.load(BOXWALLTGA);
  texman.load(CAUTIONTGA);
  texman.load(GROUNDTGA);
  texman.load(LINKSTGA);
  texman.load(PYRWALLTGA);
  texman.load(ROOFTGA);
  texman.load(WALLTGA);
  world.setTextureManager(&texman);
  modman.setTextureManager(&texman);
  world.setModelManager(&modman);
  if (!f.empty()) {
    hasname = true;
    file = f;
    world.load(file.c_str());
  } else {
    file = "";
  }
}

bool Document::file_open(const char* f) {
  file = f;
  hasname = true;
  open = true;
  unmodified = true;

  return world.load (f);
}

void Document::file_new() {
  file = "";
  hasname = false;
  open = false;
  unmodified = true;
  world.clear();
}

bool Document::file_save(const char* f) {
  if(world.write(f)) {
    unmodified = true;
    hasname = true;
    file = f;
    return true;
  }
  return false;
}

bool Document::file_save() {
  if (!hasname)
    return false;
  if(world.write(file.c_str())) {
    unmodified=true;
    return true;
  }
  return false;
}
