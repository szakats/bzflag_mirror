#ifndef __LIST_WINDOW__
#define __LIST_WINDOW__

#include <gnome.h>
#include "element.h"
#include "world.h"

namespace UI {

class MainWindow;

class ListWindow {
  public:
    ListWindow();

    void hideW();
    void showW();

    GtkWidget *getList();
    void setWorld(World *world);
    void setMain(MainWindow *mw);
  protected:
    GtkWidget *window;
    GtkWidget *swindow;
    GtkWidget *list;
};

namespace TW {

gint hide(GtkWidget *window, ListWindow *lw);
gint select_row(GtkCList *clist, gint row, gint column, GdkEventButton *event, gpointer data);
gint unselect_row(GtkCList *clist, gint row, gint column, GdkEventButton *event, gpointer data);

} // ns: UI::TW

} // ns: UI

#endif
