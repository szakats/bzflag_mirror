#ifndef __UI_MAIN_H__
#define __UI_MAIN_H__

#include <gtkgl/gtkglarea.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "document.h"
#include "callbacks.h"
#include "menus.h"
#include "toolbar.h"
#include "ui_list.h"
#include "ui_inspect.h"

namespace UI {

class MainWindow {
  public:
    MainWindow();

    void setsignals();
    void redraw();
    void select(int item);
    void select(std::string item);
    void unselectAll();
    void duplicateselected();
    void removeselected();
    void applychanges();
    void movetoselected();
    void updatetrans();
    GtkWidget *getWidget();
    
    void toggle_toolbar();
    void toggle_statusbar();
    void toggle_list();
    void toggle_inspector();

    double mouse_x, mouse_y;
    bool kshift, kcontrol;
    double transparency;
    ListWindow *list;
    InspectorWindow *inspect;
  private:
    GtkWidget *window;
    GtkWidget *toolbar;
    GtkWidget *statusbar;
    GtkWidget *glarea;

    int selected;
    bool showlist;
    bool showinspector;
    MainToolbar main_toolbar;
};

}

#endif
