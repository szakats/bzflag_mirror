#ifndef __UI_LIST_H__
#define __UI_LIST_H__

#include <gnome.h>
#include "document.h"
#include "callbacks.h"

// xpms
#include "data/stub.xpm"
#include "data/basicon.xpm"
#include "data/boxicon.xpm"
#include "data/lnkicon.xpm"
#include "data/pyricon.xpm"
#include "data/telicon.xpm"

typedef enum {
  ALL,
  BOXES,
  PYRAMIDS,
  TELEPORTERS,
  LINKS,
  BASES
} ViewType;

namespace UI {

class ListWindow {
  public:
    ListWindow();

    void setView(ViewType view);
    ViewType getView();

    void selectItem(Element e);
    void unselectAll();
    void addItem(Element e);
    void removeItem(Element e);

    GtkWidget *getWidget();

  private:
    ViewType view;

    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *pickbox;
    GtkWidget *swindow;
    GtkWidget *list;
    GtkListStore *liststore;

    GdkPixbuf *boxicon, *pyricon, *telicon, *lnkicon, *basicon;
};

}

#endif
