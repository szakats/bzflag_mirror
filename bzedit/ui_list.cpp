#include "ui_list.h"
#include "globals.h"

namespace UI {

ListWindow::ListWindow() {
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Element List");
  gtk_window_set_role(GTK_WINDOW(window), "list");
  gtk_widget_set_size_request(window, 130, 300);
  g_object_set(G_OBJECT(window), "allow-grow", FALSE, NULL);
  gtk_signal_connect(GTK_OBJECT(window), "delete_event", GTK_SIGNAL_FUNC(CB::LW::deletee), NULL);

  vbox = gtk_vbox_new(FALSE, 0);

  pickbox = gtk_combo_new();
  GList *strings = NULL;
  strings = g_list_prepend(strings, (void*) "Bases");
  strings = g_list_prepend(strings, (void*) "Links");
  strings = g_list_prepend(strings, (void*) "Teleporters");
  strings = g_list_prepend(strings, (void*) "Pyramids");
  strings = g_list_prepend(strings, (void*) "Boxes");
  strings = g_list_prepend(strings, (void*) "All");
  gtk_combo_set_popdown_strings(GTK_COMBO(pickbox), strings);
  g_list_free(strings);
//  gtk_box_pack_start(GTK_BOX(vbox), pickbox, FALSE, TRUE, 0);

  swindow = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(swindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  liststore = gtk_list_store_new(2, GDK_TYPE_PIXBUF, G_TYPE_STRING);
  list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(liststore));
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  renderer = gtk_cell_renderer_pixbuf_new();
  column = gtk_tree_view_column_new_with_attributes("", renderer, "pixbuf", 0, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);
  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("name", renderer, "text", 1, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);
  GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));
  g_signal_connect(G_OBJECT(selection), "changed", G_CALLBACK(CB::LW::change_select), NULL);

  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(swindow), list);
  gtk_box_pack_start(GTK_BOX(vbox), swindow, TRUE, TRUE, 0);

  gtk_container_add(GTK_CONTAINER(window), vbox);
  gtk_widget_show_all(window);

  g_object_set(G_OBJECT(window), "allow-grow", TRUE, NULL);

  boxicon = gdk_pixbuf_new_from_xpm_data(boxicon_xpm);
  pyricon = gdk_pixbuf_new_from_xpm_data(pyricon_xpm);
  telicon = gdk_pixbuf_new_from_xpm_data(telicon_xpm);
  lnkicon = gdk_pixbuf_new_from_xpm_data(lnkicon_xpm);
  basicon = gdk_pixbuf_new_from_xpm_data(basicon_xpm);

  view = ALL;
}

void ListWindow::setView(ViewType view) {
  this->view = view;
}

ViewType ListWindow::getView() {
  return view;
}

void ListWindow::selectItem(Element e) {
  GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));
  GtkTreeIter iter;
  GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(list));
  gchar *name;
  if(gtk_tree_model_get_iter_first(model, &iter)) {
    gtk_tree_model_get(model, &iter, 1, &name, -1);
    if(strcmp((char*) name, e.name.c_str()) == 0) {
      gtk_tree_selection_select_iter(selection, &iter);
      return;
    }
    while(gtk_tree_model_iter_next(model, &iter)) {
      gtk_tree_model_get(model, &iter, 1, &name, -1);
      if(strcmp((char*) name, e.name.c_str()) == 0) {
        gtk_tree_selection_select_iter(selection, &iter);
	return;
      }
    }
  }
}

void ListWindow::unselectAll() {
  GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));
  g_signal_handlers_block_by_func(G_OBJECT(selection), (void*) G_CALLBACK(CB::LW::change_select), NULL);
  gtk_tree_selection_unselect_all(selection);
  g_signal_handlers_unblock_by_func(G_OBJECT(selection), (void*) G_CALLBACK(CB::LW::change_select), NULL);
}

void ListWindow::addItem(Element e) {
  GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));
  GtkTreeIter iter;
  gtk_list_store_append(liststore, &iter);
  if(e.type == BOX)
    gtk_list_store_set(liststore, &iter, 0, boxicon, 1, e.name.c_str(), -1);
  else if(e.type == PYRAMID)
    gtk_list_store_set(liststore, &iter, 0, pyricon, 1, e.name.c_str(), -1);
  else if(e.type == TELEPORTER)
    gtk_list_store_set(liststore, &iter, 0, telicon, 1, e.name.c_str(), -1);
  else if(e.type == LINK)
    gtk_list_store_set(liststore, &iter, 0, lnkicon, 1, e.name.c_str(), -1);
  else if(e.type == BASE)
    gtk_list_store_set(liststore, &iter, 0, basicon, 1, e.name.c_str(), -1);
  gtk_tree_selection_select_iter(selection, &iter);
}

void ListWindow::removeItem(Element e) {
  GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));
  GtkTreeIter iter;
  GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(list));
  gchar *name;
  if(gtk_tree_model_get_iter_first(model, &iter)) {
    gtk_tree_model_get(model, &iter, 1, &name, -1);
    if(strcmp((char*) name, e.name.c_str()) == 0) {
      gtk_list_store_remove(liststore, &iter);
      return;
    }
    while(gtk_tree_model_iter_next(model, &iter)) {
      gtk_tree_model_get(model, &iter, 1, &name, -1);
      if(strcmp((char*) name, e.name.c_str()) == 0) {
	gtk_list_store_remove(liststore, &iter);
	return;
      }
    }
  }
}

GtkWidget *ListWindow::getWidget() {
  return window;
}

}
