#include "ui_edit.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream.h>
#include "textures.h"
#include "ui_main.h"

namespace UI {

EditWindow::EditWindow() {
  // Choose settings for our GTK+ GL Area
  int glargs[] = {GDK_GL_RGBA, GDK_GL_DOUBLEBUFFER, GDK_GL_NONE};

  // Create the window
  window = gtk_window_new(GTK_WINDOW_DIALOG);
  gtk_window_set_title(GTK_WINDOW(window), "Edit window");
  gtk_signal_connect(GTK_OBJECT(window), "delete_event", GTK_SIGNAL_FUNC(EW::destroy), this);

  // Create the gl area
  glarea = gtk_gl_area_new(glargs);
  if(glarea == NULL) {
    cerr << "could not allocate OpenGL context\n";
    gtk_widget_destroy(window);
  }
  gtk_widget_set_usize(GTK_WIDGET(glarea), 320, 240);
  // Add the this pointer as a data entry for the widget
  // This allows us to use events to modify camera position
  gtk_object_set_data(GTK_OBJECT(glarea), "edit_window", this);
  gtk_object_set_data(GTK_OBJECT(glarea), "camera", &camera);
  // set up events and connect signals to the gl area
  // pass this pointer as data so functions can access camera
  gtk_widget_set_events(GTK_WIDGET(glarea), GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK | GDK_KEY_PRESS_MASK);
  gtk_signal_connect(GTK_OBJECT(glarea), "realize", GTK_SIGNAL_FUNC(EW::initialize), NULL);
  gtk_signal_connect(GTK_OBJECT(glarea), "expose_event", GTK_SIGNAL_FUNC(EW::draw), NULL);
  gtk_signal_connect(GTK_OBJECT(glarea), "configure_event", GTK_SIGNAL_FUNC(EW::reshape), NULL);
  gtk_signal_connect(GTK_OBJECT(glarea), "button_press_event", GTK_SIGNAL_FUNC(EW::buttonpress), NULL);
  gtk_signal_connect(GTK_OBJECT(glarea), "key_press_event", GTK_SIGNAL_FUNC(EW::keypress), NULL);

  // allow the gl area to focus, and grab the focus within the window
  GTK_WIDGET_SET_FLAGS(glarea, GTK_CAN_FOCUS);
  gtk_widget_grab_focus(GTK_WIDGET(glarea));
  gtk_container_add(GTK_CONTAINER(window), glarea);

  // show everything
  gtk_widget_show(glarea);
  gtk_widget_show(window);

  // Initialize textures
  initTextures();

  // Make sure we are alive
  dead = false;
}

void EditWindow::Update() {
  GdkEventExpose event;
  event.count = 0;
  EW::draw(glarea, &event);
}

Camera &EditWindow::getCamera() {
  // allow access to the camera
  return camera;
}

bool EditWindow::isDead() {
  return dead;
}

void EditWindow::setDead(bool dead) {
  this->dead = dead;
}

void EditWindow::setWorld(World *world) {
  this->world = world;
  gtk_object_set_data(GTK_OBJECT(glarea), "world", world);
}

void EditWindow::setMainw(MainWindow *mw) {
  gtk_object_set_data(GTK_OBJECT(glarea), "main_window", mw);
}

namespace EW {

gint destroy(GtkWidget *window, EditWindow *ew) {
  gtk_widget_destroy(window);
  ew->setDead(true);
  return FALSE;
}

gint initialize(GtkWidget *glarea, gpointer data) {
  if(gtk_gl_area_make_current(GTK_GL_AREA(glarea))) {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glDisable(GL_CULL_FACE);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glViewport(0, 0, glarea->allocation.width, glarea->allocation.height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double) glarea->allocation.width / (double) glarea->allocation.height, 3.0, 2500.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    return TRUE;
  } else {
    cerr << "could not select OpenGL context\n";
    return FALSE;
  }
}

gint draw(GtkWidget *glarea, GdkEventExpose *event) {
  World *world = (World *) gtk_object_get_data(GTK_OBJECT(glarea), "world");
  Camera *camera = (Camera *) gtk_object_get_data(GTK_OBJECT(glarea), "camera");
  if(gtk_gl_area_make_current(GTK_GL_AREA(glarea))) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    world->render(*camera);
    gtk_gl_area_swapbuffers(GTK_GL_AREA(glarea));
    return TRUE;
  } else {
    cerr << "could not select OpenGL context\n";
    return FALSE;
  }
}

gint reshape(GtkWidget *glarea, GdkEventConfigure *event) {
  if(gtk_gl_area_make_current(GTK_GL_AREA(glarea))) {
    glViewport(0, 0, glarea->allocation.width, glarea->allocation.height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double) glarea->allocation.width / (double) glarea->allocation.height, 3.0, 2500.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    return TRUE;
  } else {
    cerr << "could not select OpenGL context\n";
    return FALSE;
  }
}

gint keypress(GtkWidget *glarea, GdkEventKey *event) {
  EditWindow *ew = (EditWindow *) gtk_object_get_data(GTK_OBJECT(glarea), "edit_window");
  Camera &camera = ew->getCamera();
  switch(event->keyval) {
    case GDK_Left:
      camera.rot += 1;
      if(camera.rot < 0)
	camera.rot += 360;
      if(camera.rot > 360)
	camera.rot -= 360;
      break;
    case GDK_Right:
      camera.rot -= 1;
      if(camera.rot < 0)
	camera.rot += 360;
      if(camera.rot > 360)
	camera.rot -= 360;
      break;
    case GDK_Up:
      if(camera.ele > 271) {
	camera.ele -= 1;
        if(camera.ele < 0)
	  camera.ele += 360;
        if(camera.ele > 360)
	  camera.ele -= 360;
      }
      break;
    case GDK_Down:
      if(camera.ele < 360) {
	camera.ele += 1;
        if(camera.ele < 0)
	  camera.ele += 360;
        if(camera.ele > 360)
	  camera.ele -= 360;
      }
      break;
    case GDK_z:
      if(camera.d < 1500)
	camera.d += 10;
      break;
    case GDK_a:
      if(camera.d > 10)
	camera.d -= 10;
      break;
  }
  // cancel default handler
  gtk_signal_emit_stop_by_name(GTK_OBJECT(glarea), "key_press_event");
  ew->Update();
  return TRUE;
}

gint buttonpress(GtkWidget *glarea, GdkEventButton *event) {
  int x = (int) event->x;
  int y = (int) event->y;
  World *world = (World *) gtk_object_get_data(GTK_OBJECT(glarea), "world");
  Camera *camera = (Camera *) gtk_object_get_data(GTK_OBJECT(glarea), "camera");
  MainWindow *mainWin = (MainWindow *) gtk_object_get_data(GTK_OBJECT(glarea), "main_window");
  GLuint *buffer;
  GLint hits;
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  buffer = new GLuint[world->size() * 4];
  glSelectBuffer(world->size() * 4, buffer);
  glRenderMode(GL_SELECT);
  glInitNames();
  glPushName(0);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluPickMatrix((double) x, (double) (viewport[3] - y), 1.0, 1.0, viewport);
  gluPerspective(45.0, (float) (viewport[2] - viewport[0]) / (float) (viewport[3] - viewport[1]), 3.0, 2500.0);
  glMatrixMode(GL_MODELVIEW);
  world->render_targets(*camera);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  hits = glRenderMode(GL_RENDER);
  if(hits > 0) {
    int choose = buffer[3];
    int depth = buffer[1];
    for(int i = 1; i < hits; i++) {
      if(buffer[i * 4 + 1] < depth) {
	choose = buffer[i * 4 + 3];
	depth = buffer[i * 4 + 1];
      }
    }
    for(int i = 0; i < world->size(); i++) {
      (*world)[i].selected = false;
    }
    (*world)[choose].selected = true;
  } else {
    for(int i = 0; i < world->size(); i++) {
      (*world)[i].selected = false;
    }
  }
  delete buffer;
  mainWin->update(MW::EDITWINS | MW::SPINVALUES | MW::SPINS | MW::LIST);

  return TRUE;
}

} // ns: UI::EW

} // ns: UI
