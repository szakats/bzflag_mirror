#include "ui_list.h"
#include "ui_main.h"

namespace UI {

ListWindow::ListWindow() {
  // Create the window
  window = gtk_window_new(GTK_WINDOW_DIALOG);
  gtk_window_set_title(GTK_WINDOW(window), "List window");
  gtk_signal_connect(GTK_OBJECT(window), "delete_event", GTK_SIGNAL_FUNC(TW::hide), this);

  // Create the scrolled window
  swindow = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(swindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(window), swindow);
  
  // Create the list
  list = gtk_clist_new(1);
  gtk_signal_connect(GTK_OBJECT(list), "select-row", GTK_SIGNAL_FUNC(TW::select_row), NULL);
  gtk_signal_connect(GTK_OBJECT(list), "unselect-row", GTK_SIGNAL_FUNC(TW::unselect_row), NULL);
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(swindow), list);

  // Show everything
  gtk_widget_show(window);
  gtk_widget_show(swindow);
  gtk_widget_show(list);
}

void ListWindow::hideW() {
  gtk_widget_hide(window);
}

void ListWindow::showW() {
  gtk_widget_show(window);
}

GtkWidget *ListWindow::getList() {
  return list;
}

void ListWindow::setWorld(World *world) {
  gtk_object_set_data(GTK_OBJECT(list), "world", world);
}

void ListWindow::setMain(MainWindow *mw) {
  gtk_object_set_data(GTK_OBJECT(list), "mainw", mw);
}

namespace TW {

gint hide(GtkWidget *window, ListWindow *lw) {
  gtk_widget_hide(window);
  return TRUE;
}

gint select_row(GtkCList *clist, gint row, gint column, GdkEventButton *event, gpointer data) {
  World *world = (World *) gtk_object_get_data(GTK_OBJECT(clist), "world");
  MainWindow *mainw = (MainWindow *) gtk_object_get_data(GTK_OBJECT(clist), "mainw");
  (*world)[row].selected = true;
  if((*world)[row].type == Element::LINK)
    mainw->showLinkWin(&((*world)[row]));
  mainw->update(MW::EDITWINS | MW::SPINVALUES);
  mainw->update(MW::SPINS);
  return TRUE;
}

gint unselect_row(GtkCList *clist, gint row, gint column, GdkEventButton *event, gpointer data) {
  World *world = (World *) gtk_object_get_data(GTK_OBJECT(clist), "world");
  MainWindow *mainw = (MainWindow *) gtk_object_get_data(GTK_OBJECT(clist), "mainw");
  (*world)[row].selected = false;
  if((*world)[row].type == Element::LINK)
    mainw->destroyLinkWin();
  mainw->update(MW::EDITWINS | MW::SPINVALUES);
  mainw->update(MW::SPINS);
  return TRUE;
}

} // ns: UI::TW

} // ns: UI
