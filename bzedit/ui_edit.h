#ifndef __EDIT_WINDOW__
#define __EDIT_WINDOW__

#include <gnome.h>
#include <gtkgl/gtkglarea.h>
#include "camera.h"
#include "world.h"

namespace UI {

class MainWindow;
  
class EditWindow {
  public:
    EditWindow();

    void Update();		// Redraws scene
    Camera &getCamera();	// Allows modifcation of camera
    bool isDead();
    void setDead(bool dead);
    void setWorld(World *world);
    void setMainw(MainWindow *mw);
  protected:
    GtkWidget *window;
    GtkWidget *glarea;
    Camera camera;
    bool dead;
    World *world;
};

namespace EW {

gint destroy(GtkWidget *window, EditWindow *ew);
gint initialize(GtkWidget *glarea, gpointer data);
gint draw(GtkWidget *glarea, GdkEventExpose *event);
gint reshape(GtkWidget *glarea, GdkEventConfigure *event);
gint keypress(GtkWidget *glarea, GdkEventKey *event);
gint buttonpress(GtkWidget *glarea, GdkEventButton *event);

} // ns: UI::EW

} // ns: UI
#endif
