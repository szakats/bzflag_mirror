#ifndef __WORLD_H__
#define __WORLD_H__

#include <gnome.h>
#include <vector>
#include <string>
#include "camera.h"
#include "element.h"

class World : public vector<Element> {
  public:
    World();
    World(World &w);
    ~World();
    void render(Camera &c);
    void render_targets(Camera &c);
    void buildList(GtkWidget *clist);
    void buildTeleporterList(GtkWidget *clist);
    void uniqueName(Element &el);
    bool load(char *filename);
    bool append(char *filename);
    bool write(char *filename);
    Element getElementByName(string name);
    Element getElementByTypeNumber(int type, int number);
    int getElementTypeNumberByName(string name);
    World operator = (World &w);
  private:
    void sort();
    GnomePixmap *boxicon;
    GnomePixmap *pyricon;
    GnomePixmap *telicon;
    GnomePixmap *lnkicon;
    Ground ground;
    Walls walls;
};

#endif
