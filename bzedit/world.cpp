#include <stdio.h>
#include <math.h>
#include <fstream.h>
#include "world.h"

World::World() {
  boxicon = GNOME_PIXMAP(gnome_pixmap_new_from_file("/usr/local/share/bzedit/data/boxicon.png"));
  pyricon = GNOME_PIXMAP(gnome_pixmap_new_from_file("/usr/local/share/bzedit/data/pyricon.png"));
  telicon = GNOME_PIXMAP(gnome_pixmap_new_from_file("/usr/local/share/bzedit/data/telicon.png"));
  lnkicon = GNOME_PIXMAP(gnome_pixmap_new_from_file("/usr/local/share/bzedit/data/lnkicon.png"));
}

World::World(World &w) {
  boxicon = w.boxicon;
  pyricon = w.pyricon;
  telicon = w.telicon;
  lnkicon = w.lnkicon;
  (vector<Element>) (*this) = (vector<Element>) w;
}

World::~World() {
  gtk_widget_destroy(GTK_WIDGET(boxicon));
  gtk_widget_destroy(GTK_WIDGET(pyricon));
  gtk_widget_destroy(GTK_WIDGET(telicon));
  gtk_widget_destroy(GTK_WIDGET(lnkicon));
}

void World::render(Camera &c) {
  ground.render(c);
  walls.render(c);
  bool transparent = false;
  vector<Element>::iterator it = begin();
  while(it != end()) {
    if(it->selected)
      transparent = true;
    it++;
  }
  it = begin();
  while(it != end()) {
    it->render(c, (transparent && (!it->selected)));
    if(it->type == Element::LINK && it->selected)
      it->l->render(c, this);
    it++;
  }
}

void World::render_targets(Camera &c) {
  vector<Element>::iterator it = begin();
  for(unsigned int i = 0; i < size(); i++) {
    glLoadName(i);
    (*this)[i].render(c, false);
  }
}

void World::buildList(GtkWidget *clist) {
//  sort();
  gchar *text[] = {"", NULL};
  
  gtk_clist_freeze(GTK_CLIST(clist));
  gtk_clist_clear(GTK_CLIST(clist));
  for(int i = 0; i < size(); i++) {
    gtk_clist_append(GTK_CLIST(clist), text);
  }
  vector<Element>::iterator it = begin();
  int row = 0;
  while(it != end()) {
    if(it->type == Element::BOX) {
      gtk_clist_set_pixtext(GTK_CLIST(clist), row, 0, it->name.c_str(), 8, boxicon->pixmap, boxicon->mask);
    } else if(it->type == Element::PYRAMID) {
      gtk_clist_set_pixtext(GTK_CLIST(clist), row, 0, it->name.c_str(), 8, pyricon->pixmap, pyricon->mask);
    } else if(it->type == Element::TELEPORTER) {
      gtk_clist_set_pixtext(GTK_CLIST(clist), row, 0, it->name.c_str(), 8, telicon->pixmap, telicon->mask);
    } else if(it->type == Element::LINK) {
      gtk_clist_set_pixtext(GTK_CLIST(clist), row, 0, it->name.c_str(), 8, lnkicon->pixmap, lnkicon->mask);
    } else {
      gtk_clist_set_text(GTK_CLIST(clist), row, 0, "Unknown object type!");
    }
    if(it->selected) {
      gtk_clist_select_row(GTK_CLIST(clist), row, 0);
    }
    it++;
    row++;
  }
  gtk_clist_thaw(GTK_CLIST(clist));
}

void World::buildTeleporterList(GtkWidget *clist) {
  gchar *text[] = {"", "", NULL};

  gtk_clist_freeze(GTK_CLIST(clist));
  gtk_clist_clear(GTK_CLIST(clist));
  for(int i = 0; i < size(); i++) {
    if((*this)[i].type == Element::TELEPORTER) {
      gtk_clist_append(GTK_CLIST(clist), text);
      gtk_clist_append(GTK_CLIST(clist), text);
    }
  }
  int row = 0;
  for(int i = 0; i < size(); i++) {
    if((*this)[i].type == Element::TELEPORTER) {
      gtk_clist_set_text(GTK_CLIST(clist), row, 0, (*this)[i].name.c_str());
      gtk_clist_set_text(GTK_CLIST(clist), row, 1, "0");
      row++;
      gtk_clist_set_text(GTK_CLIST(clist), row, 0, (*this)[i].name.c_str());
      gtk_clist_set_text(GTK_CLIST(clist), row, 1, "1");
      row++;
    }
  }
  gtk_clist_thaw(GTK_CLIST(clist));
}

void World::uniqueName(Element &el) {
  // This is horribly inefficient. Come up with something better!
  string base;
  char name[1024];
  char temp[1024];
  int num = 0;
  bool addnum = true;
  bool conflicts = false;
  bool tmpconflicts = false;
  if(el.name != "") {
    for(int i = 0; i < size(); i++) {
      if(el.name == (*this)[i].name) {
        tmpconflicts = true;
      }
    }
    conflicts = tmpconflicts;
    if(conflicts) {
      char value;
      value = el.name.c_str()[strlen(el.name.c_str()) - 1];
      for(int i = strlen(el.name.c_str()) - 1; i >= 0; i--) {
        value = el.name.c_str()[i];
        if(!(value >= '0' && value <= '9')) {
	  strncpy(temp, el.name.c_str(), i + 1);
	  temp[i + 1] = '\0';
	  addnum = false;
	  break;
        }
      }
      base = temp;
    } else {
      return;
    }
  } else if(el.type == Element::BOX) {
    base = "box";
  } else if(el.type == Element::PYRAMID) {
    base = "pyramid";
  } else if(el.type == Element::TELEPORTER) {
    base = "teleporter";
  } else if(el.type == Element::LINK) {
    base = "link";
  } else {
    base = "unknown";
  }
  conflicts = false;
  tmpconflicts = false;
  if(size() == 0) {
    sprintf(name, "%s", base.c_str());
  }
  if(addnum == false) {
    for(int i = 0; i < size(); i++) {
      if(base == (*this)[i].name) {
        tmpconflicts = true;
	break;
      }
    }
    conflicts = tmpconflicts;
  } else {
    conflicts = true;
  }
  while(conflicts) {
    num++;
    sprintf(name, "%s%i", base.c_str(), num);
    tmpconflicts = false;
    for(int i = 0; i < size(); i++) {
      if(string(name) == (*this)[i].name) {
	tmpconflicts = true;
        break;
      }
    }
    conflicts = tmpconflicts;
  }
  el.name = name;
}

bool World::load(char *filename) {
  fstream file;
  char temp;
  char temp2[82];
  char temp3[80];
  string x;
  string tmp;
  string nextname = "";
  Element *el;

  file.open(filename, ios::in);
  if(!file)
    return false;
  clear();
  while(!file.eof()) {
    file >> x;
    if(x == "#!name") {
      file >> x;
      nextname = x;
    } else if(x == "box") {
      el = new Element;
      el->makeBox();
      while(x != "end") {
	if(x == "position") {
	  file >> x;
	  el->b->set_px(atof(x.c_str()));
	  file >> x;
	  el->b->set_py(atof(x.c_str()));
	  file >> x;
	  el->b->set_pz(atof(x.c_str()));
	} else if(x == "rotation") {
	  file >> x;
	  el->b->set_angle(atof(x.c_str()) * 180 / M_PI);
	} else if(x == "size") {
	  file >> x;
	  el->b->set_sx(atof(x.c_str()));
	  file >> x;
	  el->b->set_sy(atof(x.c_str()));
	  file >> x;
	  el->b->set_sz(atof(x.c_str()));
	}
	file >> x;
      }
      if(nextname != "") {
	el->name = nextname;
	nextname = "";
      }
      uniqueName(*el);
      push_back(*el);
      delete el;
      continue;
    } else if(x == "pyramid") {
      el = new Element;
      el->makePyramid();
      while(x != "end") {
	if(x == "position") {
	  file >> x;
	  el->p->set_px(atof(x.c_str()));
	  file >> x;
	  el->p->set_py(atof(x.c_str()));
	  file >> x;
	  el->p->set_pz(atof(x.c_str()));
	} else if(x == "rotation") {
	  file >> x;
	  el->p->set_angle(atof(x.c_str()) * 180 / M_PI);
	} else if(x == "size") {
	  file >> x;
	  el->p->set_sx(atof(x.c_str()));
	  file >> x;
	  el->p->set_sy(atof(x.c_str()));
	  file >> x;
	  el->p->set_sz(atof(x.c_str()));
	}
	file >> x;
      }
      if(nextname != "") {
	cout << "Creating pyramid, name = " << nextname << endl;
	el->name = nextname;
	nextname = "";
      }
      uniqueName(*el);
      push_back(*el);
      delete el;
      continue;
    } else if(x == "teleporter") {
      el = new Element;
      el->makeTeleporter();
      while(x != "end") {
	if(x == "position") {
	  file >> x;
	  el->t->set_px(atof(x.c_str()));
	  file >> x;
	  el->t->set_py(atof(x.c_str()));
	  file >> x;
	  el->t->set_pz(atof(x.c_str()));
	} else if(x == "rotation") {
	  file >> x;
	  el->t->set_angle(atof(x.c_str()) * 180 / M_PI + 90);
	} else if(x == "size") {
	  file >> x;
	  el->t->set_sy(atof(x.c_str()));
	  file >> x;
	  el->t->set_sx(atof(x.c_str()));
	  file >> x;
	  el->t->set_sz(atof(x.c_str()));
	} else if(x == "border") {
	  file >> x;
	  el->t->set_border(atof(x.c_str()));
	}
	file >> x;
      }
      if(nextname != "") {
	cout << "Creating teleporter, name = " << nextname << endl;
	el->name = nextname;
	nextname = "";
      }
      uniqueName(*el);
      push_back(*el);
      delete el;
      continue;
    } else if(x == "link") {
      el = new Element;
      el->makeLink();
      while(x != "end") {
	if(x == "to") {
	  file >> x;
	  el->l->set_to_side((atoi(x.c_str()) % 2));
	  el->l->set_to(getElementByTypeNumber(Element::TELEPORTER, (int) (atoi(x.c_str()) / 2)).name);
	}
	if(x == "from") {
	  file >> x;
	  el->l->set_from_side((atoi(x.c_str()) % 2));
	  el->l->set_from(getElementByTypeNumber(Element::TELEPORTER, (int) (atoi(x.c_str()) / 2)).name);
	}
	file >> x;
      }
      if(nextname != "") {
	el->name = nextname;
	nextname = "";
      }
      uniqueName(*el);
      push_back(*el);
      delete el;
    } else if(x.c_str()[0] == '#') {
      file.get(temp2, 82);
      file.ignore(82, '\n');
    } else {
      // parse error
    }
  }
  file.close();
  return true;
}

bool World::append(char *filename) {
  fstream file;
  char temp;
  char temp2[82];
  char temp3[80];
  string x;
  string tmp;
  string nextname;
  Element *el;

  file.open(filename, ios::in);
  if(!file)
    return false;
  while(!file.eof()) {
    file >> x;
    if(x == "#!name") {
      file >> x;
      nextname = x;
    } else if(x == "box") {
      el = new Element;
      el->makeBox();
      while(x != "end") {
	if(x == "position") {
	  file >> x;
	  el->b->set_px(atof(x.c_str()));
	  file >> x;
	  el->b->set_py(atof(x.c_str()));
	  file >> x;
	  el->b->set_pz(atof(x.c_str()));
	} else if(x == "rotation") {
	  file >> x;
	  el->b->set_angle(atof(x.c_str()) * 180 / M_PI);
	} else if(x == "size") {
	  file >> x;
	  el->b->set_sx(atof(x.c_str()));
	  file >> x;
	  el->b->set_sy(atof(x.c_str()));
	  file >> x;
	  el->b->set_sz(atof(x.c_str()));
	}
	file >> x;
      }
      if(nextname != "") {
	el->name = nextname;
	nextname = "";
      }
      uniqueName(*el);
      push_back(*el);
      delete el;
    } else if(x == "pyramid") {
      el = new Element;
      el->makePyramid();
      while(x != "end") {
	if(x == "position") {
	  file >> x;
	  el->p->set_px(atof(x.c_str()));
	  file >> x;
	  el->p->set_py(atof(x.c_str()));
	  file >> x;
	  el->p->set_pz(atof(x.c_str()));
	} else if(x == "rotation") {
	  file >> x;
	  el->p->set_angle(atof(x.c_str()) * 180 / M_PI);
	} else if(x == "size") {
	  file >> x;
	  el->p->set_sx(atof(x.c_str()));
	  file >> x;
	  el->p->set_sy(atof(x.c_str()));
	  file >> x;
	  el->p->set_sz(atof(x.c_str()));
	}
	file >> x;
      }
      if(nextname != "") {
	el->name = nextname;
	nextname = "";
      }
      uniqueName(*el);
      push_back(*el);
      delete el;
    } else if(x == "teleporter") {
      el = new Element;
      el->makeTeleporter();
      while(x != "end") {
	if(x == "position") {
	  file >> x;
	  el->t->set_px(atof(x.c_str()));
	  file >> x;
	  el->t->set_py(atof(x.c_str()));
	  file >> x;
	  el->t->set_pz(atof(x.c_str()));
	} else if(x == "rotation") {
	  file >> x;
	  el->t->set_angle(atof(x.c_str()) * 180 / M_PI + 90);
	} else if(x == "size") {
	  file >> x;
	  el->t->set_sy(atof(x.c_str()));
	  file >> x;
	  el->t->set_sx(atof(x.c_str()));
	  file >> x;
	  el->t->set_sz(atof(x.c_str()));
	} else if(x == "border") {
	  file >> x;
	  el->t->set_border(atof(x.c_str()));
	}
	file >> x;
      }
      if(nextname != "") {
	el->name = nextname;
	nextname = "";
      }
      uniqueName(*el);
      push_back(*el);
      delete el;
    } else if(x == "link") {
      el = new Element;
      el->makeLink();
      while(x != "end") {
	if(x == "to") {
	  file >> x;
	  el->l->set_to_side((atoi(x.c_str()) % 2));
	  el->l->set_to(getElementByTypeNumber(Element::TELEPORTER, (int) (atoi(x.c_str()) / 2)).name);
	}
	if(x == "from") {
	  file >> x;
	  el->l->set_from_side((atoi(x.c_str()) % 2));
	  el->l->set_from(getElementByTypeNumber(Element::TELEPORTER, (int) (atoi(x.c_str()) / 2)).name);
	}
	file >> x;
      }
      if(nextname != "") {
	el->name = nextname;
	nextname = "";
      }
      uniqueName(*el);
      push_back(*el);
      delete el;
    } else if(x.c_str()[0] == '#') {
      file.get(temp2, 82);
      file.ignore(82, '\n');
    } else {
      // parse error
    }
  }
  file.close();
  return true;
}

bool World::write(char *filename) {
  fstream file;

  file.open(filename, ios::out);
  if(!file)
    return false;
  file << "# World built with bzedit, available at\n";
  file << "# http://jupiter.babylonia.flatirons.org/\n\n";
  for(unsigned int i = 0; i < size(); i++) {
    file << "#!name " << (*this)[i].name << endl;
    if((*this)[i].type != Element::LINK) {
      file << (*this)[i];
    } else {
      int from = getElementTypeNumberByName((*this)[i].l->get_from()) * 2;
      from += ((*this)[i].l->get_from_side());
      int to = getElementTypeNumberByName((*this)[i].l->get_to()) * 2;
      to += ((*this)[i].l->get_to_side());
      file << "link\n";
      file << "from " << from << endl;
      file << "to " << to << endl;
      file << "end\n\n";
    }
  }
  return true;
}

Element World::getElementByName(string name) {
  for(int i = 0; i < size(); i++) {
    if((*this)[i].name == name)
      return (*this)[i];
  }
  return Element();
}

void World::sort() {
  World neww;

  for(int i = 0; i < size(); i++) {
    if((*this)[i].type == Element::BOX) {
      neww.push_back((*this)[i]);
    }
  }
  for(int i = 0; i < size(); i++) {
    if((*this)[i].type == Element::PYRAMID) {
      neww.push_back((*this)[i]);
    }
  }
  for(int i = 0; i < size(); i++) {
    if((*this)[i].type == Element::TELEPORTER) {
      neww.push_back((*this)[i]);
    }
  }
  for(int i = 0; i < size(); i++) {
    if((*this)[i].type == Element::LINK) {
      neww.push_back((*this)[i]);
    }
  }
  neww.boxicon = boxicon;
  neww.pyricon = pyricon;
  neww.telicon = telicon;
  neww.lnkicon = lnkicon;
  *this = neww;
}

World World::operator = (World &w) {
  boxicon = w.boxicon;
  pyricon = w.pyricon;
  telicon = w.telicon;
  lnkicon = w.lnkicon;
  (vector<Element>) (*this) = (vector<Element>) w;
  return *this;
}

Element World::getElementByTypeNumber(int type, int number) {
  int num = 0;
  for(int i = 0; i < size(); i++) {
    if((*this)[i].type == type) {
      if(num == number) {
	return (*this)[i];
      } else {
        num++;
      }
    }
  }
  return Element();
}

int World::getElementTypeNumberByName(string name) {
  Element el = getElementByName(name);
  int num = 0;
  for(int i = 0; i < size(); i++) {
    if((*this)[i].type == el.type) {
      if((*this)[i].name == name) {
	return num;
      } else {
	num++;
      }
    }
  }
}
