#ifndef __WORLD_H__
#define __WORLD_H__

#include <gnome.h>
#include <vector>
#include "camera.h"
#include "element.h"

class World : public vector<Element> {
  public:
    World();
    void render(Camera &c);
    void render_targets(Camera &c);
    void buildList(GtkWidget *clist);
    void uniqueName(Element &el);
    bool load(char *filename);
    bool append(char *filename);
    bool write(char *filename);
    Element getElementByName(string name);
  private:
    GnomePixmap *boxicon;
    GnomePixmap *pyricon;
    GnomePixmap *telicon;
    Ground ground;
    Walls walls;
};

#endif
