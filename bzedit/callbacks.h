#ifndef __CALLBACKS_H__
#define __CALLBACKS_H__

#include <gtk/gtk.h>
#include <gtkgl/gtkglarea.h>
#include <GL/gl.h>
#include <GL/glu.h>

namespace CB {

// generic callback
gint gc(GtkWidget *widget, gpointer data);

namespace MW { // main window callbacks
gint initialize(GtkWidget *glarea, gpointer data);
gint draw(GtkWidget *glarea, GdkEventExpose *event);
gint reshape(GtkWidget *glarea, GdkEventConfigure *event);
gint keypress(GtkWidget *glarea, GdkEventKey *event);
gint keyrelease(GtkWidget *glarea, GdkEventKey *event);
gint buttonpress(GtkWidget *glarea, GdkEventButton *event);
gint mousemotion(GtkWidget *glarea, GdkEventMotion *event);
gint scroll(GtkWidget *glarea, GdkEventScroll *event);

gint newf(GtkWidget *widget, gpointer data);
gint open(GtkWidget *widget, gpointer data);
gint openok(GtkWidget *widget, GtkFileSelection *fsel);
gint really(GtkWidget *widget, gpointer data);
gint save(GtkWidget *widget, gpointer data);
gint saveas(GtkWidget *widget, gpointer data);
gint saveasok(GtkWidget *widget, GtkFileSelection *fsel);
gint saveascancel(GtkWidget *widget, GtkFileSelection *fsel);
gint exit(GtkWidget *widget, gpointer data);

gint undo(GtkWidget *widget, gpointer data);
gint redo(GtkWidget *widget, gpointer data);
gint cut(GtkWidget *widget, gpointer data);
gint copy(GtkWidget *widget, gpointer data);
gint paste(GtkWidget *widget, gpointer data);

gint vgrid(GtkWidget *widget, gpointer data);
gint vlist(GtkWidget *widget, gpointer data);
gint vinspect(GtkWidget *widget, gpointer data);
gint movetsel(GtkWidget *widget, gpointer data);

gint convtod(GtkWidget *widget, gpointer data);
gint convtor(GtkWidget *widget, gpointer data);

gint about(GtkWidget *window, gpointer data);

gint tb_zplus(GtkWidget *widget, gpointer data);
gint tb_yplus(GtkWidget *widget, gpointer data);
gint tb_yminus(GtkWidget *widget, gpointer data);
gint tb_xplus(GtkWidget *widget, gpointer data);
gint tb_xminus(GtkWidget *widget, gpointer data);
gint tb_iso(GtkWidget *widget, gpointer data);
gint tb_target(GtkWidget *widget, gpointer data);
gint tb_addbox(GtkWidget *widget, gpointer data);
gint tb_addpyr(GtkWidget *widget, gpointer data);
gint tb_addtel(GtkWidget *widget, gpointer data);
gint tb_addlnk(GtkWidget *widget, gpointer data);
gint tb_addbas(GtkWidget *widget, gpointer data);
gint tb_trans(GtkWidget *widget, gpointer data);
}

namespace LW { // list window callbacks
gint change_select(GtkTreeSelection *selection, gpointer data);
gint deletee(GtkWidget *widget, gpointer data);
}

namespace IW { // inspector window callbacks
gint duplicate(GtkWidget *widget, gpointer data);
gint remove(GtkWidget *widget, gpointer data);
gint apply(GtkWidget *widget, gpointer data);
gint deletee(GtkWidget *widget, gpointer data);
}

}

#endif
