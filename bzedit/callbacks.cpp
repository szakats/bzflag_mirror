#include "callbacks.h"
#include "globals.h"

namespace CB {

namespace MW {

gint initialize(GtkWidget *glarea, gpointer data) {
  if(gtk_gl_area_make_current(GTK_GL_AREA(glarea))) {
    GLfloat light_ambient[] = {0.5, 0.5, 0.5, 1.0};
    GLfloat light_diffuse[] = {0.75, 0.75, 0.75, 1.0};
    GLfloat light_position[] = {400, 400, 400, 1.0};

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);

    glClearColor(0, 0, 0, 0);
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    glViewport(0, 0, glarea->allocation.width, glarea->allocation.height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double) glarea->allocation.width / (double) glarea->allocation.height, 3.0, 2500.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    gtk_gl_area_swapbuffers(GTK_GL_AREA(glarea));

    return TRUE;
  } else {
    // could not set context
    return FALSE;
  }
}

gint draw(GtkWidget *glarea, GdkEventExpose *event) {
  if(gtk_gl_area_make_current(GTK_GL_AREA(glarea))) {
    mainwin->redraw();

    return TRUE;
  } else {
    // could not set context
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

    mainwin->redraw();

    return TRUE;
  } else {
    // could not set context
    return FALSE;
  }
}

gint keypress(GtkWidget *glarea, GdkEventKey *event) {
  if(event->keyval == GDK_Shift_L) {
    cout << "key pressed: left shift\n";
    mainwin->kshift = true;
  }
  if(event->keyval == GDK_Control_L) {
    cout << "key pressed: left control\n";
    mainwin->kcontrol = true;
  }
  return TRUE;
}

gint keyrelease(GtkWidget *glarea, GdkEventKey *event) {
  if(event->keyval == GDK_Shift_L) {
    cout << "key released: left shift\n";
    mainwin->kshift = false;
  }
  if(event->keyval == GDK_Control_L) {
    cout << "key released: left control\n";
    mainwin->kcontrol = false;
  }
  return TRUE;
}

gint buttonpress(GtkWidget *glarea, GdkEventButton *event) {
  int x = (int) event->x;
  int y = (int) event->y;
  GLuint *buffer;
  GLint hits;
  GLint viewport[4];
  switch(event->button) {
    case 1:
      // button 1, do picking
      glGetIntegerv(GL_VIEWPORT, viewport);
      buffer = new GLuint[doc->world.list.size() * 4];
      glSelectBuffer(doc->world.list.size() * 4, buffer);
      glRenderMode(GL_SELECT);
      glInitNames();
      glPushName(0);
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      gluPickMatrix((double) x, (double) (viewport[3] - y), 1.0, 1.0, viewport);
      gluPerspective(45.0, (float) (viewport[2] - viewport[0]) / (float) (viewport[3] - viewport[1]), 3.0, 2500.0);
      glMatrixMode(GL_MODELVIEW);
      doc->world.render_targets();
      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
      hits = glRenderMode(GL_RENDER);
      if(hits > 0) {
	int choose = buffer[3];
	int depth = buffer[1];
	for(int i = 0; i < hits; i++) {
	  if(buffer[i * 4 + 1] < depth) {
	    choose = buffer[i * 4 + 3];
	    depth = buffer[i * 4 + 1];
	  }
	}
	choose--;
	mainwin->select(choose);
      } else {
	mainwin->unselectAll();
      }
      delete buffer;
      break;
    case 2:
      // button 2, pick and center
      glGetIntegerv(GL_VIEWPORT, viewport);
      buffer = new GLuint[doc->world.list.size() * 4];
      glSelectBuffer(doc->world.list.size() * 4, buffer);
      glRenderMode(GL_SELECT);
      glInitNames();
      glPushName(0);
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      gluPickMatrix((double) x, (double) (viewport[3] - y), 1.0, 1.0, viewport);
      gluPerspective(45.0, (float) (viewport[2] - viewport[0]) / (float) (viewport[3] - viewport[1]), 3.0, 2500.0);
      glMatrixMode(GL_MODELVIEW);
      doc->world.render_targets();
      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glMatrixMode(GL_MODELVIEW);
      hits = glRenderMode(GL_RENDER);
      if(hits > 0) {
	int choose = buffer[3];
	int depth = buffer[1];
	for(int i = 0; i < hits; i++) {
	  if(buffer[i * 4 + 1] < depth) {
	    choose = buffer[i * 4 + 3];
	    depth = buffer[i * 4 + 1];
	  }
	}
	choose--;
	if(doc->world.list[choose].type != LINK) {
	  doc->camera.fx = doc->world.list[choose].obj->get_px();
	  doc->camera.fy = doc->world.list[choose].obj->get_py();
	  doc->camera.fz = doc->world.list[choose].obj->get_pz();
	  doc->camera.fcamera = true;
	}
	mainwin->select(choose);
      } else {
	doc->camera.fx = 0;
	doc->camera.fy = 0;
	doc->camera.fz = 0;
	doc->camera.fcamera = true;
	mainwin->unselectAll();
      }
      delete buffer;
      break;
    case 3:
      // button 3, start roaming
      mainwin->mouse_x = event->x_root;
      mainwin->mouse_y = event->y_root;
      break;
    case 4:
      // button 4, mouse wheel up, zoom in
      break;
    case 5:
      // button 5, mouse wheel down, zoom out
      break;
  }
  return TRUE;
}

gint mousemotion(GtkWidget *glarea, GdkEventMotion *event) {
  double deltax = event->x_root - mainwin->mouse_x;
  double deltay = event->y_root - mainwin->mouse_y;
  mainwin->mouse_x = event->x_root;
  mainwin->mouse_y = event->y_root;
  if(mainwin->kshift && mainwin->kcontrol) {
    doc->camera.d += deltay;
    if(doc->camera.d < 10)
      doc->camera.d = 10;
    if(doc->camera.d > 1500)
      doc->camera.d = 1500;
  } else if(mainwin->kshift) {
  } else if(mainwin->kcontrol) {
  } else {
    doc->camera.rot += deltax / 5;
    doc->camera.ele += deltay / 5;
    if(doc->camera.ele > 0)
      doc->camera.ele = 0;
    if(doc->camera.ele < -90)
      doc->camera.ele = -90;
  }
  mainwin->redraw();
  return TRUE;
}

gint scroll(GtkWidget *widget, GdkEventScroll *event) {
  if(event->direction == GDK_SCROLL_UP) {
    if(doc->camera.d > 10)
      doc->camera.d -= 40;
    mainwin->redraw();
  } else if(event->direction == GDK_SCROLL_DOWN) {
    if(doc->camera.d < 1500)
      doc->camera.d += 40;
    mainwin->redraw();
  }
}

// Let's make believe this is used as a callback, when it isn't
gint really (GtkWidget *widget, gpointer data) {
  string msg = ((string) "Save changes to ") + (doc->file.empty() ? "new file": "'"+doc->file+"'") + "?";
  if(!doc->unmodified) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons ("Unsaved Changes",
                                GTK_WINDOW(mainwin->getWidget()),
                                (GtkDialogFlags)(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
                                GTK_STOCK_YES,
                                GTK_RESPONSE_YES,
                                GTK_STOCK_NO,
                                GTK_RESPONSE_NO,
                                GTK_STOCK_CANCEL,
                                GTK_RESPONSE_CANCEL,
                                NULL);

    GtkWidget *label = gtk_label_new(msg.c_str());
 
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), label, TRUE, TRUE, 10);
   
    gtk_widget_show (label);
    gtk_widget_show (dialog);
   
    gint result = gtk_dialog_run (GTK_DIALOG(dialog));
    gtk_widget_destroy (dialog);
   
    switch (result) {
      case GTK_RESPONSE_CANCEL:
        return FALSE;
      case GTK_RESPONSE_YES:
        save(widget,data);
        if (!doc->unmodified)
          return FALSE;
        break;
    }
  }
  return TRUE;
}

gint open(GtkWidget *widget, gpointer data) {
  if (!really(widget,data))
    return TRUE;
  GtkFileSelection *fsel = GTK_FILE_SELECTION(gtk_file_selection_new("Open"));
  gtk_signal_connect(GTK_OBJECT(fsel->ok_button), "clicked", GTK_SIGNAL_FUNC(openok), fsel);
  gtk_signal_connect_object(GTK_OBJECT(fsel->cancel_button), "clicked", GTK_SIGNAL_FUNC(gtk_widget_destroy), GTK_OBJECT(fsel));
  gtk_window_set_transient_for(GTK_WINDOW(fsel), GTK_WINDOW(mainwin->getWidget()));
  gtk_widget_show(GTK_WIDGET(fsel));
  return TRUE;
}

gint openok(GtkWidget *widget, GtkFileSelection *fsel) {
  const char *fname;
  fname = gtk_file_selection_get_filename(fsel);
  if(!fname || !doc->file_open(fname)) {
    GtkWidget *dlg = gnome_error_dialog_parented("Cannot open file for reading", GTK_WINDOW(fsel));
    gtk_window_set_modal(GTK_WINDOW(dlg), TRUE);
  } else {
    gtk_widget_destroy(GTK_WIDGET(fsel));
    mainwin->list->setView(ALL);
    mainwin->redraw();
  }
  return TRUE;
}

gint newf(GtkWidget *widget, gpointer data) {
  if (!really(widget,data))
    return TRUE;
  doc->file_new();
  mainwin->list->setView(ALL);
  mainwin->redraw();
  return TRUE;
}

gint save(GtkWidget *widget, gpointer data) {
  if(!doc->hasname)
    return saveas(widget, data);
  doc->file_save();
  if (doc->exiting)
    gtk_main_quit();
  
  return TRUE;
}

gint saveas(GtkWidget *widget, gpointer data) {
  GtkFileSelection *fsel = GTK_FILE_SELECTION(gtk_file_selection_new("Save As"));
  gtk_signal_connect(GTK_OBJECT(fsel->ok_button), "clicked", GTK_SIGNAL_FUNC(saveasok), fsel);
  gtk_signal_connect_object(GTK_OBJECT(fsel->cancel_button), "clicked", GTK_SIGNAL_FUNC(saveascancel), GTK_OBJECT(fsel));
  gtk_window_set_transient_for(GTK_WINDOW(fsel), GTK_WINDOW(mainwin->getWidget()));
  gtk_widget_show(GTK_WIDGET(fsel));
  return TRUE;
}

gint saveasok(GtkWidget *widget, GtkFileSelection *fsel) {
  const char *fname;
  fname = gtk_file_selection_get_filename(fsel);
  if(!fname || !doc->file_save(fname)) {
    GtkWidget *dlg = gnome_error_dialog_parented("Cannot open file for writing", GTK_WINDOW(fsel));
    gtk_window_set_modal(GTK_WINDOW(dlg), TRUE);
  } else {
    gtk_widget_destroy(GTK_WIDGET(fsel));
	cout << doc-> exiting << endl;
	if (doc->exiting)
	  gtk_main_quit();
  }
  return TRUE;
}

gint saveascancel(GtkWidget *widget, GtkFileSelection *fsel) {
  doc->exiting = false;
  gtk_widget_destroy(widget);
  return TRUE;
}

gint exit(GtkWidget *widget, gpointer data) {
  if (!really(widget,data))
  {
    doc->exiting = true;
    return TRUE;
  }
  cout << doc->unmodified << endl;
  gtk_main_quit();
  return FALSE;
}

gint undo(GtkWidget *widget, gpointer data) {
  return TRUE;
}

gint redo(GtkWidget *widget, gpointer data) {
  return TRUE;
}

gint cut(GtkWidget *widget, gpointer data) {
  return TRUE;
}

gint copy(GtkWidget *widget, gpointer data) {
  return TRUE;
}

gint paste(GtkWidget *widget, gpointer data) {
  return TRUE;
}

gint vgrid(GtkWidget *widget, gpointer data) {
  return TRUE;
}

gint vlist(GtkWidget *widget, gpointer data) {
  if(doc)
    mainwin->toggle_list();
  return TRUE;
}

gint vinspect(GtkWidget *widget, gpointer data) {
  if(doc)
    mainwin->toggle_inspector();
  return TRUE;
}

gint movetsel(GtkWidget *widget, gpointer data) {
  mainwin->movetoselected();
  return TRUE;
}

gint convtod(GtkWidget *widget, gpointer data) {
  ElementList::iterator it = doc->world.list.begin();
  while(it != doc->world.list.end()) {
    it->obj->set_angle(it->obj->get_angle() * M_RAD2DEG);
    it->obj->init();
    it++;
  }
  return TRUE;
}

gint convtor(GtkWidget *widget, gpointer data) {
  ElementList::iterator it = doc->world.list.begin();
  while(it != doc->world.list.end()) {
    it->obj->set_angle(it->obj->get_angle() * M_DEG2RAD);
    it->obj->init();
    it++;
  }
  return TRUE;
}

gint about(GtkWidget *window, gpointer data) {
  static GtkWidget *dialog = NULL;
  const char *authors[] = {
    "David Trowbridge",
    "Dan Kuester",
    "Jeff Myers",
    NULL
  };
  const char *other[] = {
    "These people helped with random",
    "things (not documentation). So,",
    "special thanks goes out to",
    "m0nkey_b0y, Gerbil, Quinn Buchanan",
    "Steve Houston, and others",
    NULL
  };

  if(dialog) {
    gdk_window_show(dialog->window);
    gdk_window_raise(dialog->window);
    return TRUE;
  } 
  dialog = gnome_about_new("bzedit", "2.1", "(c) 2002 David Trowbridge", "\"panic while you still can\"", authors, other, NULL, NULL);
  // TODO: set parent (necessary?)
  gtk_signal_connect(GTK_OBJECT(dialog), "destroy", GTK_SIGNAL_FUNC(gtk_widget_destroyed), &dialog);
  gtk_widget_show(dialog);
  return TRUE;
}

gint tb_zplus(GtkWidget *widget, gpointer data) {
  doc->camera.fx = 0;
  doc->camera.fy = 0;
  doc->camera.fz = 0;
  doc->camera.d = 750;
  doc->camera.rot = 0;
  doc->camera.ele = 0;
  doc->camera.fcamera = true;
  mainwin->redraw();
  return TRUE;
}

gint tb_yplus(GtkWidget *widget, gpointer data) {
  doc->camera.fx = 0;
  doc->camera.fy = 0;
  doc->camera.fz = 0;
  doc->camera.d = 750;
  doc->camera.rot = 0;
  doc->camera.ele = -90;
  doc->camera.fcamera = true;
  mainwin->redraw();
  return TRUE;
}

gint tb_yminus(GtkWidget *widget, gpointer data) {
  doc->camera.fx = 0;
  doc->camera.fy = 0;
  doc->camera.fz = 0;
  doc->camera.d = 750;
  doc->camera.rot = 180;
  doc->camera.ele = -90;
  doc->camera.fcamera = true;
  mainwin->redraw();
  return TRUE;
}

gint tb_xplus(GtkWidget *widget, gpointer data) {
  doc->camera.fx = 0;
  doc->camera.fy = 0;
  doc->camera.fz = 0;
  doc->camera.d = 750;
  doc->camera.rot = 90;
  doc->camera.ele = -90;
  doc->camera.fcamera = true;
  mainwin->redraw();
  return TRUE;
}

gint tb_xminus(GtkWidget *widget, gpointer data) {
  doc->camera.fx = 0;
  doc->camera.fy = 0;
  doc->camera.fz = 0;
  doc->camera.d = 750;
  doc->camera.rot = 270;
  doc->camera.ele = -90;
  doc->camera.fcamera = true;
  mainwin->redraw();
  return TRUE;
}

gint tb_iso(GtkWidget *widget, gpointer data) {
  doc->camera.fx = 0;
  doc->camera.fy = 0;
  doc->camera.fz = 0;
  doc->camera.d = 750;
  doc->camera.rot = 45;
  doc->camera.ele = 285;
  doc->camera.fcamera = true;
  mainwin->redraw();
  return TRUE;
}

gint tb_target(GtkWidget *widget, gpointer data) {
  doc->camera.fx = 0;
  doc->camera.fy = 0;
  doc->camera.fz = 0;
  doc->camera.fcamera = true;
  mainwin->redraw();
  return TRUE;
}

gint tb_addbox(GtkWidget *widget, gpointer data) {
  int item = doc->world.addElement(BOX);
  mainwin->list->addItem(*(doc->world.getElement(item)));
  mainwin->redraw();
  doc->unmodified = false;
  return TRUE;
}

gint tb_addpyr(GtkWidget *widget, gpointer data) {
  int item = doc->world.addElement(PYRAMID);
  mainwin->list->addItem(*(doc->world.getElement(item)));
  mainwin->redraw();
  doc->unmodified = false;
  return TRUE;
}

gint tb_addtel(GtkWidget *widget, gpointer data) {
  int item = doc->world.addElement(TELEPORTER);
  mainwin->list->addItem(*(doc->world.getElement(item)));
  mainwin->redraw();
  doc->unmodified = false;
  return TRUE;
}

gint tb_addlnk(GtkWidget *widget, gpointer data) {
  int item = doc->world.addElement(LINK);
  mainwin->list->addItem(*(doc->world.getElement(item)));
  mainwin->redraw();
  doc->unmodified = false;
  return TRUE;
}

gint tb_addbas(GtkWidget *widget, gpointer data) {
  int item = doc->world.addElement(BASE);
  mainwin->list->addItem(*(doc->world.getElement(item)));
  mainwin->redraw();
  doc->unmodified = false;
  return TRUE;
}

gint tb_trans(GtkWidget *widget, gpointer data) {
  mainwin->updatetrans();
  return TRUE;
}

}

namespace LW {

gint change_select(GtkTreeSelection *selection, gpointer data) {
  GtkTreeIter iter;
  GtkTreeModel *model;
  if(gtk_tree_selection_get_selected(selection, &model, &iter)) {
    gchar *name;
    gtk_tree_model_get(GTK_TREE_MODEL(model), &iter, 1, &name, -1);
    mainwin->select(std::string(name));
  } else {
    mainwin->unselectAll();
  }
  return TRUE;
}

gint deletee(GtkWidget *widget, gpointer data) {
  mainwin->toggle_list();
  return TRUE;
}

}

namespace IW {

gint duplicate(GtkWidget *widget, gpointer data) {
  doc->unmodified=false;
  mainwin->duplicateselected();
}

gint remove(GtkWidget *widget, gpointer data) {
  doc->unmodified=false;
  mainwin->removeselected();
}

gint apply(GtkWidget *widget, gpointer data) {
  doc->unmodified=false;
  mainwin->applychanges();
}

gint deletee(GtkWidget *widget, gpointer data) {
  doc->unmodified=false;
  mainwin->toggle_inspector();
  return TRUE;
}

}

gint gc(GtkWidget *widget, gpointer data) {
  return TRUE;
}

}
