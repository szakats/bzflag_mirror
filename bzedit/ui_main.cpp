#include "ui_main.h"
#include <gnome.h>
#include <stdio.h>
#include <iostream.h>

namespace UI {

MainWindow::MainWindow() {
  // Create the window, menus, and statusbar
  window = gnome_app_new("bzedit", "BZFlag Editor");
  statusbar = gnome_appbar_new(FALSE, TRUE, GNOME_PREFERENCES_USER);
  gtk_signal_connect(GTK_OBJECT(window), "delete_event", GTK_SIGNAL_FUNC(MW::quit), NULL);
  gnome_app_create_menus(GNOME_APP(window), MW::main_menu);
  gnome_app_set_statusbar(GNOME_APP(window), statusbar);
  gnome_app_install_menu_hints(GNOME_APP(window), MW::main_menu);

  gtk_object_set_data(GTK_OBJECT(MW::file_menu[0].widget), "main_window", this);
  gtk_object_set_data(GTK_OBJECT(MW::file_menu[2].widget), "main_window", this);
  gtk_object_set_data(GTK_OBJECT(MW::file_menu[3].widget), "main_window", this);
  gtk_object_set_data(GTK_OBJECT(MW::file_menu[4].widget), "main_window", this);
  gtk_object_set_data(GTK_OBJECT(MW::file_menu[5].widget), "main_window", this);
  gtk_object_set_data(GTK_OBJECT(MW::file_menu[6].widget), "main_window", this);
  gtk_object_set_data(GTK_OBJECT(MW::file_menu[7].widget), "main_window", this);
  gtk_object_set_data(GTK_OBJECT(MW::eAdd_menu[0].widget), "main_window", this);
  gtk_object_set_data(GTK_OBJECT(MW::eAdd_menu[1].widget), "main_window", this);
  gtk_object_set_data(GTK_OBJECT(MW::eAdd_menu[2].widget), "main_window", this);
  gtk_object_set_data(GTK_OBJECT(MW::eAdd_menu[3].widget), "main_window", this);
  gtk_object_set_data(GTK_OBJECT(MW::edit_menu[0].widget), "main_window", this);
  gtk_object_set_data(GTK_OBJECT(MW::edit_menu[1].widget), "main_window", this);
  gtk_object_set_data(GTK_OBJECT(MW::edit_menu[3].widget), "main_window", this);
  gtk_object_set_data(GTK_OBJECT(MW::edit_menu[4].widget), "main_window", this);
  gtk_object_set_data(GTK_OBJECT(MW::edit_menu[5].widget), "main_window", this);
  gtk_object_set_data(GTK_OBJECT(MW::edit_menu[6].widget), "main_window", this);
  gtk_object_set_data(GTK_OBJECT(MW::edit_menu[7].widget), "main_window", this);
  gtk_object_set_data(GTK_OBJECT(MW::windows_menu[0].widget), "main_window", this);
  gtk_object_set_data(GTK_OBJECT(MW::help_menu[0].widget), "main_window", this);

  // Create and fill the table
  table = gtk_table_new(11, 3, FALSE);
  // label: Position
  lpos = gtk_label_new("Position");
  gtk_table_attach_defaults(GTK_TABLE(table), lpos, 0, 3, 0, 1);
  // spin: position-x
  sa = (GtkAdjustment *) gtk_adjustment_new(0.0, -400.0, 400.0, 0.1, 1.0, 1.0);
  sbpx = gtk_spin_button_new(sa, 0.1, 1);
  gtk_signal_connect(GTK_OBJECT(sbpx), "changed", GTK_SIGNAL_FUNC(MW::chpx), NULL);
  gtk_widget_set_sensitive(sbpx, FALSE);
  gtk_table_attach_defaults(GTK_TABLE(table), sbpx, 0, 1, 1, 2);
  gtk_object_set_data(GTK_OBJECT(sbpx), "main_window", this);
  // spin: position-y
  sa = (GtkAdjustment *) gtk_adjustment_new(0.0, -400.0, 400.0, 0.1, 1.0, 1.0);
  sbpy = gtk_spin_button_new(sa, 0.1, 1);
  gtk_signal_connect(GTK_OBJECT(sbpy), "changed", GTK_SIGNAL_FUNC(MW::chpy), NULL);
  gtk_widget_set_sensitive(sbpy, FALSE);
  gtk_table_attach_defaults(GTK_TABLE(table), sbpy, 1, 2, 1, 2);
  gtk_object_set_data(GTK_OBJECT(sbpy), "main_window", this);
  // spin: position-z
  sa = (GtkAdjustment *) gtk_adjustment_new(0.0, 0.0, 1500.0, 0.1, 1.0, 1.0);
  sbpz = gtk_spin_button_new(sa, 0.1, 1);
  gtk_signal_connect(GTK_OBJECT(sbpz), "changed", GTK_SIGNAL_FUNC(MW::chpz), NULL);
  gtk_widget_set_sensitive(sbpz, FALSE);
  gtk_table_attach_defaults(GTK_TABLE(table), sbpz, 2, 3, 1, 2);
  gtk_object_set_data(GTK_OBJECT(sbpz), "main_window", this);
  // label: Rotation
  lrot = gtk_label_new("Rotation");
  gtk_table_attach_defaults(GTK_TABLE(table), lrot, 0, 3, 2, 3);
  // spin: rotation
  sa = (GtkAdjustment *) gtk_adjustment_new(0.0, 0.0, 360.0, 0.1, 1.0, 1.0);
  sbangle = gtk_spin_button_new(sa, 0.1, 1);
  gtk_signal_connect(GTK_OBJECT(sbangle), "changed", GTK_SIGNAL_FUNC(MW::changle), NULL);
  gtk_widget_set_sensitive(sbangle, FALSE);
  gtk_table_attach_defaults(GTK_TABLE(table), sbangle, 0, 3, 3, 4);
  gtk_object_set_data(GTK_OBJECT(sbangle), "main_window", this);
  // label: size
  lsize = gtk_label_new("Size");
  gtk_table_attach_defaults(GTK_TABLE(table), lsize, 0, 3, 4, 5);
  // spin: size-x
  sa = (GtkAdjustment *) gtk_adjustment_new(0.0, 0.0, 1200.0, 0.1, 1.0, 1.0);
  sbsx = gtk_spin_button_new(sa, 0.1, 1);
  gtk_signal_connect(GTK_OBJECT(sbsx), "changed", GTK_SIGNAL_FUNC(MW::chsx), NULL);
  gtk_widget_set_sensitive(sbsx, FALSE);
  gtk_table_attach_defaults(GTK_TABLE(table), sbsx, 0, 1, 5, 6);
  gtk_object_set_data(GTK_OBJECT(sbsx), "main_window", this);
  // spin: size-y
  sa = (GtkAdjustment *) gtk_adjustment_new(0.0, 0.0, 1200.0, 0.1, 1.0, 1.0);
  sbsy = gtk_spin_button_new(sa, 0.1, 1);
  gtk_signal_connect(GTK_OBJECT(sbsy), "changed", GTK_SIGNAL_FUNC(MW::chsy), NULL);
  gtk_widget_set_sensitive(sbsy, FALSE);
  gtk_table_attach_defaults(GTK_TABLE(table), sbsy, 1, 2, 5, 6);
  gtk_object_set_data(GTK_OBJECT(sbsy), "main_window", this);
  // spin: size-z
  sa = (GtkAdjustment *) gtk_adjustment_new(0.0, 0.0, 3000.0, 0.1, 1.0, 1.0);
  sbsz = gtk_spin_button_new(sa, 0.1, 1);
  gtk_signal_connect(GTK_OBJECT(sbsz), "changed", GTK_SIGNAL_FUNC(MW::chsz), NULL);
  gtk_widget_set_sensitive(sbsz, FALSE);
  gtk_table_attach_defaults(GTK_TABLE(table), sbsz, 2, 3, 5, 6);
  gtk_object_set_data(GTK_OBJECT(sbsz), "main_window", this);
  // label: border
  lbor = gtk_label_new("Border");
  gtk_table_attach_defaults(GTK_TABLE(table), lbor, 0, 3, 6, 7);
  // spin: border
  sa = (GtkAdjustment *) gtk_adjustment_new(1.0, 0.0, 20.0, 0.1, 1.0, 1.0);
  sbborder = gtk_spin_button_new(sa, 0.1, 1);
  gtk_signal_connect(GTK_OBJECT(sbborder), "changed", GTK_SIGNAL_FUNC(MW::chborder), NULL);
  gtk_widget_set_sensitive(sbborder, FALSE);
  gtk_table_attach_defaults(GTK_TABLE(table), sbborder, 0, 3, 7, 8);
  gtk_object_set_data(GTK_OBJECT(sbborder), "main_window", this);
  // label: name
  lname = gtk_label_new("Name");
  gtk_table_attach_defaults(GTK_TABLE(table), lname, 0, 3, 8, 9);
  // entry: name
  enname = gtk_entry_new();
  gtk_signal_connect(GTK_OBJECT(enname), "changed", GTK_SIGNAL_FUNC(MW::chname), NULL);
  gtk_object_set_data(GTK_OBJECT(enname), "main_window", this);
  gtk_widget_set_sensitive(enname, FALSE);
  gtk_table_attach_defaults(GTK_TABLE(table), enname, 0, 3, 9, 10);
  // button: test
  testworld = gtk_button_new_with_label("Test World");
  gtk_signal_connect(GTK_OBJECT(testworld), "clicked", GTK_SIGNAL_FUNC(MW::testw), this);
  gtk_table_attach_defaults(GTK_TABLE(table), testworld, 0, 3, 10, 11);

  lw.setWorld(world);
  lw.setMain(this);

  gnome_app_set_contents(GNOME_APP(window), table);
  gtk_widget_show_all(window);

  world = NULL;
}

MainWindow::~MainWindow() {
  for(int i = 0; i < edits.size(); i++) {
    delete edits[i];
  }
}

void MainWindow::enable_position() {
  gtk_widget_set_sensitive(sbpx, TRUE);
  gtk_widget_set_sensitive(sbpy, TRUE);
  gtk_widget_set_sensitive(sbpz, TRUE);
}

void MainWindow::enable_rotation() {
  gtk_widget_set_sensitive(sbangle, TRUE);
}

void MainWindow::enable_size() {
  gtk_widget_set_sensitive(sbsx, TRUE);
  gtk_widget_set_sensitive(sbsy, TRUE);
  gtk_widget_set_sensitive(sbsz, TRUE);
}

void MainWindow::enable_border() {
  gtk_widget_set_sensitive(sbborder, TRUE);
}

void MainWindow::enable_size(int which) {
  if(which & MW::SIZE_X)
    gtk_widget_set_sensitive(sbsx, TRUE);
  if(which & MW::SIZE_Y)
    gtk_widget_set_sensitive(sbsy, TRUE);
  if(which & MW::SIZE_Z)
    gtk_widget_set_sensitive(sbsz, TRUE);
}

void MainWindow::enable_name() {
  gtk_widget_set_sensitive(enname, TRUE);
}

void MainWindow::enable_most() {
  enable_position();
  enable_rotation();
  enable_size();
  enable_name();
  disable_border();
}

void MainWindow::enable_tele() {
  enable_position();
  enable_rotation();
  enable_size(MW::SIZE_X | MW::SIZE_Z);
  enable_name();
  enable_border();
}

void MainWindow::disable_position() {
  gtk_widget_set_sensitive(sbpx, FALSE);
  gtk_widget_set_sensitive(sbpy, FALSE);
  gtk_widget_set_sensitive(sbpz, FALSE);
}

void MainWindow::disable_rotation() {
  gtk_widget_set_sensitive(sbangle, FALSE);
}

void MainWindow::disable_size() {
  gtk_widget_set_sensitive(sbsx, FALSE);
  gtk_widget_set_sensitive(sbsy, FALSE);
  gtk_widget_set_sensitive(sbsz, FALSE);
}

void MainWindow::disable_border() {
  gtk_widget_set_sensitive(sbborder, FALSE);
}

void MainWindow::disable_size(int which) {
  if(which & MW::SIZE_X)
    gtk_widget_set_sensitive(sbsx, FALSE);
  if(which & MW::SIZE_Y)
    gtk_widget_set_sensitive(sbsy, FALSE);
  if(which & MW::SIZE_Z)
    gtk_widget_set_sensitive(sbsz, FALSE);
}

void MainWindow::disable_name() {
  gtk_widget_set_sensitive(enname, FALSE);
}

void MainWindow::disable_all() {
  disable_position();
  disable_rotation();
  disable_size();
  disable_border();
  disable_name();
}

void MainWindow::update(short updates) {
  if(updates & MW::EDITWINS) {
    for(unsigned int i = 0; i < edits.size(); i++) {
      edits[i]->Update();
    }
  }
  bool select = false;
  bool multiselect = false;
  for(unsigned int i = 0; i < world->size(); i++) {
    if(select && (*world)[i].selected)
      multiselect = true;
    if((*world)[i].selected)
      select = true;
  }
  if(select && !multiselect) {
    for(unsigned int i = 0; i < world->size(); i++) {
      if((*world)[i].selected) {
	if(updates & MW::SPINVALUES) {
	  gtk_signal_handler_block_by_func(GTK_OBJECT(enname), GTK_SIGNAL_FUNC(MW::chname), NULL);
	  gtk_entry_set_text(GTK_ENTRY(enname), (*world)[i].name.c_str());
	  gtk_signal_handler_unblock_by_func(GTK_OBJECT(enname), GTK_SIGNAL_FUNC(MW::chname), NULL);
	}
	switch((*world)[i].type) {
	  case Element::BOX:
	    if(updates & MW::SPINVALUES) {
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(sbpx), (*world)[i].b->get_px());
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(sbpy), (*world)[i].b->get_py());
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(sbpz), (*world)[i].b->get_pz());
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(sbsx), (*world)[i].b->get_sx());
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(sbsy), (*world)[i].b->get_sy());
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(sbsz), (*world)[i].b->get_sz());
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(sbangle), (*world)[i].b->get_angle());
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(sbborder), 0.0);
	    }
	    if(updates & MW::SPINS) {
	      enable_most();
	    }
	    break;
	  case Element::PYRAMID:
	    if(updates & MW::SPINVALUES) {
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(sbpx), (*world)[i].p->get_px());
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(sbpy), (*world)[i].p->get_py());
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(sbpz), (*world)[i].p->get_pz());
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(sbsx), (*world)[i].p->get_sx());
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(sbsy), (*world)[i].p->get_sy());
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(sbsz), (*world)[i].p->get_sz());
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(sbangle), (*world)[i].p->get_angle());
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(sbborder), 0.0);
	    }
	    if(updates & MW::SPINS) {
	      enable_most();
	    }
	    break;
	  case Element::TELEPORTER:
	    if(updates & MW::SPINVALUES) {
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(sbpx), (*world)[i].t->get_px());
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(sbpy), 0.0);
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(sbpz), (*world)[i].t->get_pz());
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(sbsx), (*world)[i].t->get_sx());
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(sbsy), (*world)[i].t->get_sy());
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(sbsz), (*world)[i].t->get_sz());
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(sbangle), (*world)[i].t->get_angle());
	      gtk_spin_button_set_value(GTK_SPIN_BUTTON(sbborder), (*world)[i].t->get_border());
	    }
	    if(updates & MW::SPINS) {
	      enable_tele();
	    }
	    break;
	}
	if(updates & MW::EDITWINS) {
	  for(unsigned int j = 0; j < edits.size(); j++) {
	    if((*world)[i].type == Element::BOX) {
	      edits[j]->getCamera().fx = (*world)[i].b->get_px();
	      edits[j]->getCamera().fy = (*world)[i].b->get_py();
	      edits[j]->getCamera().fz = (*world)[i].b->get_pz();
	    } else if((*world)[i].type == Element::PYRAMID) {
	      edits[j]->getCamera().fx = (*world)[i].p->get_px();
	      edits[j]->getCamera().fy = (*world)[i].p->get_py();
	      edits[j]->getCamera().fz = (*world)[i].p->get_pz();
	    } else if((*world)[i].type == Element::TELEPORTER) {
	      edits[j]->getCamera().fx = (*world)[i].t->get_px();
	      edits[j]->getCamera().fy = (*world)[i].t->get_py();
	      edits[j]->getCamera().fz = (*world)[i].t->get_pz();
	    }
	  }
	}
      }
    }
  } else if(updates & MW::SPINS) {
    disable_all();
  }
  if(updates & MW::LIST) {
    world->buildList(lw.getList());
  }
}

void MainWindow::addEW(EditWindow *ew) {
  ew->setWorld(world);
  ew->setMainw(this);
  edits.push_back(ew);
  update(MW::EDITWINS);
}

GtkWidget *MainWindow::getWindow() {
  return window;
}

void MainWindow::setWorld(World *world) {
  this->world = world;
  lw.setWorld(world);
}

World *MainWindow::getWorld() {
  return world;
}

void MainWindow::showList() {
  lw.showW();
}

ListWindow &MainWindow::getListWindow() {
  return lw;
}

void MainWindow::showLinkWin(Element *link) {
  GtkWidget *dialog, *ok, *cancel, *vbox, *hbox;
  GtkWidget *sw1, *sw2, *clist1, *clist2;
  gchar *titles[2] = {"Teleporter", "Side"};

  dialog = gtk_dialog_new();
  
  sw1 = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw1), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_widget_show(sw1);
  clist1 = gtk_clist_new_with_titles(2, titles);
  gtk_object_set_data(GTK_OBJECT(clist1), "selection", 0);
  gtk_signal_connect(GTK_OBJECT(clist1), "select-row", GTK_SIGNAL_FUNC(MW::sellink), NULL);
  gtk_signal_connect(GTK_OBJECT(clist1), "unselect-row", GTK_SIGNAL_FUNC(MW::unsellink), NULL);
  world->buildTeleporterList(clist1);
  gtk_widget_show(clist1);
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(sw1), clist1);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), sw1, TRUE, TRUE, 0);
  sw2 = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw2), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_widget_show(sw2);
  clist2 = gtk_clist_new_with_titles(2, titles);
  gtk_object_set_data(GTK_OBJECT(clist2), "selection", 0);
  gtk_signal_connect(GTK_OBJECT(clist2), "select-row", GTK_SIGNAL_FUNC(MW::sellink), NULL);
  gtk_signal_connect(GTK_OBJECT(clist2), "unselect-row", GTK_SIGNAL_FUNC(MW::unsellink), NULL);
  world->buildTeleporterList(clist2);
  gtk_widget_show(clist2);
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(sw2), clist2);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), sw2, TRUE, TRUE, 0);

  ok = gnome_stock_button(GNOME_STOCK_BUTTON_OK);
  gtk_object_set_data(GTK_OBJECT(ok), "clist1", clist1);
  gtk_object_set_data(GTK_OBJECT(ok), "clist2", clist2);
  gtk_object_set_data(GTK_OBJECT(ok), "dialog", dialog);
  gtk_object_set_data(GTK_OBJECT(ok), "mainw", this);
  gtk_widget_show(ok);
  gtk_signal_connect(GTK_OBJECT(ok), "clicked", GTK_SIGNAL_FUNC(MW::linkok), link);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->action_area), ok, TRUE, TRUE, 0);
  cancel = gnome_stock_button(GNOME_STOCK_BUTTON_CANCEL);
  gtk_widget_show(cancel);
  gtk_signal_connect_object(GTK_OBJECT(cancel), "clicked", GTK_SIGNAL_FUNC(gtk_widget_destroy), GTK_OBJECT(dialog));
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->action_area), cancel, TRUE, TRUE, 0);
  gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
  gtk_widget_show(dialog);

  linkwin = dialog;
}

void MainWindow::destroyLinkWin() {
  gtk_widget_destroy(linkwin);
}

namespace MW {

gint quit(GtkWidget *window, gpointer data) {
  gtk_main_quit();
  return FALSE;
}

gint newf(GtkWidget *window, gpointer data) {
  return TRUE;
}

gint neww(GtkWidget *window, gpointer data) {
  MainWindow *mainWin = (MainWindow *) gtk_object_get_data(GTK_OBJECT(window), "main_window");
  EditWindow *newwin = new EditWindow();
  mainWin->addEW(newwin);
  return TRUE;
}

gint open(GtkWidget *window, gpointer data) {
  MainWindow *mainWin = (MainWindow *) gtk_object_get_data(GTK_OBJECT(window), "main_window");
  GtkFileSelection *fsel = GTK_FILE_SELECTION(gtk_file_selection_new("Open"));
  gtk_object_set_data(GTK_OBJECT(fsel), "main_window", mainWin);
  gtk_object_set_data(GTK_OBJECT(fsel), "world", mainWin->getWorld());
  gtk_signal_connect(GTK_OBJECT(fsel->ok_button), "clicked", GTK_SIGNAL_FUNC(openok), fsel);
  gtk_signal_connect_object(GTK_OBJECT(fsel->cancel_button), "clicked", GTK_SIGNAL_FUNC(gtk_widget_destroy), GTK_OBJECT(fsel));
  gtk_window_set_transient_for(GTK_WINDOW(fsel), GTK_WINDOW(mainWin->getWindow()));
  gtk_widget_show(GTK_WIDGET(fsel));
  return TRUE;
}

gint openok(GtkWidget *button, GtkFileSelection *fsel) {
  MainWindow *mainWin = (MainWindow *) gtk_object_get_data(GTK_OBJECT(fsel), "main_window");
  World *world = (World *) gtk_object_get_data(GTK_OBJECT(fsel), "world");
  char *fname;
  fname = gtk_file_selection_get_filename(fsel);
  if(!fname || !world->load(fname)) {
    GtkWidget *dlg = gnome_error_dialog_parented("Cannot open file for reading", GTK_WINDOW(fsel));
    gtk_window_set_modal(GTK_WINDOW(dlg), TRUE);
  } else {
    gtk_widget_destroy(GTK_WIDGET(fsel));
    mainWin->update(EDITWINS | SPINVALUES | SPINS | LIST);
  }
  return TRUE;
}

gint save(GtkWidget *window, gpointer data) {
  return TRUE;
}

gint saveas(GtkWidget *window, gpointer data) {
  MainWindow *mainWin = (MainWindow *) gtk_object_get_data(GTK_OBJECT(window), "main_window");
  GtkFileSelection *fsel = GTK_FILE_SELECTION(gtk_file_selection_new("Save As"));
  gtk_object_set_data(GTK_OBJECT(fsel), "main_window", mainWin);
  gtk_object_set_data(GTK_OBJECT(fsel), "world", mainWin->getWorld());
  gtk_signal_connect(GTK_OBJECT(fsel->ok_button), "clicked", GTK_SIGNAL_FUNC(saveasok), fsel);
  gtk_signal_connect_object(GTK_OBJECT(fsel->cancel_button), "clicked", GTK_SIGNAL_FUNC(gtk_widget_destroy), GTK_OBJECT(fsel));
  gtk_window_set_transient_for(GTK_WINDOW(fsel), GTK_WINDOW(mainWin->getWindow()));
  gtk_widget_show(GTK_WIDGET(fsel));
  return TRUE;
}

gint saveasok(GtkWidget *button, GtkFileSelection *fsel) {
  MainWindow *mainWin = (MainWindow *) gtk_object_get_data(GTK_OBJECT(fsel), "main_window");
  World *world = (World *) gtk_object_get_data(GTK_OBJECT(fsel), "world");
  char *fname;
  fname = gtk_file_selection_get_filename(fsel);
  if(!fname || !world->write(fname)) {
    GtkWidget *dlg = gnome_error_dialog_parented("Cannot open file for writing", GTK_WINDOW(fsel));
    gtk_window_set_modal(GTK_WINDOW(dlg), TRUE);
  } else {
    gtk_widget_destroy(GTK_WIDGET(fsel));
  }
  return TRUE;
}

gint import(GtkWidget *window, gpointer data) {
  MainWindow *mainWin = (MainWindow *) gtk_object_get_data(GTK_OBJECT(window), "main_window");
  GtkFileSelection *fsel = GTK_FILE_SELECTION(gtk_file_selection_new("Import"));
  gtk_object_set_data(GTK_OBJECT(fsel), "main_window", mainWin);
  gtk_object_set_data(GTK_OBJECT(fsel), "world", mainWin->getWorld());
  gtk_signal_connect(GTK_OBJECT(fsel->ok_button), "clicked", GTK_SIGNAL_FUNC(importok), fsel);
  gtk_signal_connect_object(GTK_OBJECT(fsel->cancel_button), "clicked", GTK_SIGNAL_FUNC(gtk_widget_destroy), GTK_OBJECT(fsel));
  gtk_window_set_transient_for(GTK_WINDOW(fsel), GTK_WINDOW(mainWin->getWindow()));
  gtk_widget_show(GTK_WIDGET(fsel));
  return TRUE;
}

gint importok(GtkWidget *button, GtkFileSelection *fsel) {
  MainWindow *mainWin = (MainWindow *) gtk_object_get_data(GTK_OBJECT(fsel), "main_window");
  World *world = (World *) gtk_object_get_data(GTK_OBJECT(fsel), "world");
  char *fname;
  fname = gtk_file_selection_get_filename(fsel);
  if(!fname || !world->append(fname)) {
    GtkWidget *dlg = gnome_error_dialog_parented("Cannot open file for readming", GTK_WINDOW(fsel));
    gtk_window_set_modal(GTK_WINDOW(dlg), TRUE);
  } else {
    gtk_widget_destroy(GTK_WIDGET(fsel));
    mainWin->update(EDITWINS | SPINVALUES | SPINS | LIST);
  }
  return TRUE;
}

gint abox(GtkWidget *window, gpointer data) {
  MainWindow *mainWin = (MainWindow *) gtk_object_get_data(GTK_OBJECT(window), "main_window");
  ListWindow &lw = mainWin->getListWindow();
  GtkWidget *list = lw.getList();
  Element box;
  box.makeBox();
  for(unsigned int i = 0; i < mainWin->getWorld()->size(); i++) {
    (*(mainWin->getWorld()))[i].selected = false;
  }
  box.selected = true;
  mainWin->getWorld()->uniqueName(box);
  mainWin->getWorld()->push_back(box);
  mainWin->getWorld()->buildList(list);
  mainWin->update(MW::SPINVALUES | MW::LIST | MW::EDITWINS | MW::LIST);
  return TRUE;
}

gint apyr(GtkWidget *window, gpointer data) {
  MainWindow *mainWin = (MainWindow *) gtk_object_get_data(GTK_OBJECT(window), "main_window");
  ListWindow &lw = mainWin->getListWindow();
  GtkWidget *list = lw.getList();
  Element pyramid;
  pyramid.makePyramid();
  for(unsigned int i = 0; i < mainWin->getWorld()->size(); i++) {
    (*(mainWin->getWorld()))[i].selected = false;
  }
  pyramid.selected = true;
  mainWin->getWorld()->uniqueName(pyramid);
  mainWin->getWorld()->push_back(pyramid);
  mainWin->getWorld()->buildList(list);
  mainWin->update(MW::SPINVALUES | MW::LIST | MW::EDITWINS | MW::LIST);
  return TRUE;
}

gint atel(GtkWidget *window, gpointer data) {
  MainWindow *mainWin = (MainWindow *) gtk_object_get_data(GTK_OBJECT(window), "main_window");
  ListWindow &lw = mainWin->getListWindow();
  GtkWidget *list = lw.getList();
  Element teleporter;
  teleporter.makeTeleporter();
  for(unsigned int i = 0; i < mainWin->getWorld()->size(); i++) {
    (*(mainWin->getWorld()))[i].selected = false;
  }
  teleporter.selected = true;
  mainWin->getWorld()->uniqueName(teleporter);
  mainWin->getWorld()->push_back(teleporter);
  mainWin->update(MW::SPINVALUES | MW::LIST | MW::EDITWINS | MW::LIST);
  return TRUE;
}

gint alnk(GtkWidget *window, gpointer data) {
  MainWindow *mainWin = (MainWindow *) gtk_object_get_data(GTK_OBJECT(window), "main_window");
  ListWindow &lw = mainWin->getListWindow();
  GtkWidget *list = lw.getList();
  Element link;
  link.makeLink();
  for(unsigned int i = 0; i < mainWin->getWorld()->size(); i++) {
    (*(mainWin->getWorld()))[i].selected = false;
  }
  link.selected = true;
  mainWin->getWorld()->uniqueName(link);
  mainWin->getWorld()->push_back(link);
  mainWin->update(MW::SPINVALUES | MW::LIST | MW::EDITWINS | MW::LIST);
  mainWin->showLinkWin(&(*mainWin->getWorld())[mainWin->getWorld()->size() - 1]);
  return TRUE;
}

gint undo(GtkWidget *window, gpointer data) {
  return TRUE;
}

gint redo(GtkWidget *window, gpointer data) {
  return TRUE;
}

gint cut(GtkWidget *window, gpointer data) {
  return TRUE;
}

gint copy(GtkWidget *window, gpointer data) {
  return TRUE;
}

gint paste(GtkWidget *window, gpointer data) {
  return TRUE;
}

gint clear(GtkWidget *window, gpointer data) {
  return TRUE;
}

gint selall(GtkWidget *window, gpointer data) {
  return TRUE;
}

gint about(GtkWidget *window, gpointer data) {
  MainWindow *mainWin = (MainWindow *) gtk_object_get_data(GTK_OBJECT(window), "main_window");
  static GtkWidget *dialog = NULL;
  const char *authors[] = {
    "David Trowbridge",
    NULL
  };
  if(dialog) {
    gdk_window_show(dialog->window);
    gdk_window_raise(dialog->window);
    return TRUE;
  }
  dialog = gnome_about_new("bzedit", "2.0a", "(c) 2001 David Trowbridge", authors, "Editor for the popular game BZFlag\nWritten out of utter boredom.\nSpecial thanks to:\n  m0nkey_b0y, Gerbil, Quinn Buchanan,\n  Steve Houston, and others.", NULL);
  // TODO: set parent
  gtk_signal_connect(GTK_OBJECT(dialog), "destroy", GTK_SIGNAL_FUNC(gtk_widget_destroyed), &dialog);
  gtk_widget_show(dialog);
  return TRUE;
}

gint chpx(GtkWidget *sbpx, gpointer data) {
  MainWindow *mainWin = (MainWindow *) gtk_object_get_data(GTK_OBJECT(sbpx), "main_window");
  vector<Element>::iterator it = mainWin->getWorld()->begin();
  while(!(it->selected))
    it++;
  if(it->type == Element::BOX) {
    it->b->set_px(gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(sbpx)));
  } else if(it->type == Element::PYRAMID) {
    it->p->set_px(gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(sbpx)));
  } else {
    it->t->set_px(gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(sbpx)));
  }
  mainWin->update(EDITWINS);
  return TRUE;
}

gint chpy(GtkWidget *sbpy, gpointer data) {
  MainWindow *mainWin = (MainWindow *) gtk_object_get_data(GTK_OBJECT(sbpy), "main_window");
  vector<Element>::iterator it = mainWin->getWorld()->begin();
  while(!(it->selected))
    it++;
  if(it->type == Element::BOX) {
    it->b->set_py(gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(sbpy)));
  } else if(it->type == Element::PYRAMID) {
    it->p->set_py(gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(sbpy)));
  } else {
    it->t->set_py(gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(sbpy)));
  }
  mainWin->update(EDITWINS);
  return TRUE;
}

gint chpz(GtkWidget *sbpz, gpointer data) {
  MainWindow *mainWin = (MainWindow *) gtk_object_get_data(GTK_OBJECT(sbpz), "main_window");
  vector<Element>::iterator it = mainWin->getWorld()->begin();
  while(!(it->selected))
    it++;
  if(it->type == Element::BOX) {
    it->b->set_pz(gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(sbpz)));
  } else if(it->type == Element::PYRAMID) {
    it->p->set_pz(gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(sbpz)));
  } else {
    it->t->set_pz(gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(sbpz)));
  }
  mainWin->update(EDITWINS);
  return TRUE;
}

gint changle(GtkWidget *sbangle, gpointer data) {
  MainWindow *mainWin = (MainWindow *) gtk_object_get_data(GTK_OBJECT(sbangle), "main_window");
  vector<Element>::iterator it = mainWin->getWorld()->begin();
  while(!(it->selected))
    it++;
  if(it->type == Element::BOX) {
    it->b->set_angle(gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(sbangle)));
  } else if(it->type == Element::PYRAMID) {
    it->p->set_angle(gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(sbangle)));
  } else {
    it->t->set_angle(gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(sbangle)));
  }
  mainWin->update(EDITWINS);
  return TRUE;
}

gint chsx(GtkWidget *sbsx, gpointer data) {
  MainWindow *mainWin = (MainWindow *) gtk_object_get_data(GTK_OBJECT(sbsx), "main_window");
  vector<Element>::iterator it = mainWin->getWorld()->begin();
  while(!(it->selected))
    it++;
  if(it->type == Element::BOX) {
    it->b->set_sx(gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(sbsx)));
  } else if(it->type == Element::PYRAMID) {
    it->p->set_sx(gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(sbsx)));
  } else {
    it->t->set_sx(gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(sbsx)));
  }
  mainWin->update(EDITWINS);
  return TRUE;
}

gint chsy(GtkWidget *sbsy, gpointer data) {
  MainWindow *mainWin = (MainWindow *) gtk_object_get_data(GTK_OBJECT(sbsy), "main_window");
  vector<Element>::iterator it = mainWin->getWorld()->begin();
  while(!(it->selected))
    it++;
  if(it->type == Element::BOX) {
    it->b->set_sy(gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(sbsy)));
  } else if(it->type == Element::PYRAMID) {
    it->p->set_sy(gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(sbsy)));
  } else {
    it->t->set_sy(gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(sbsy)));
  }
  mainWin->update(EDITWINS);
  return TRUE;
}

gint chsz(GtkWidget *sbsz, gpointer data) {
  MainWindow *mainWin = (MainWindow *) gtk_object_get_data(GTK_OBJECT(sbsz), "main_window");
  vector<Element>::iterator it = mainWin->getWorld()->begin();
  while(!(it->selected))
    it++;
  if(it->type == Element::BOX) {
    it->b->set_sz(gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(sbsz)));
  } else if(it->type == Element::PYRAMID) {
    it->p->set_sz(gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(sbsz)));
  } else {
    it->t->set_sz(gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(sbsz)));
  }
  mainWin->update(EDITWINS);
  return TRUE;
}

gint chborder(GtkWidget *sbborder, gpointer data) {
  MainWindow *mainWin = (MainWindow *) gtk_object_get_data(GTK_OBJECT(sbborder), "main_window");
  vector<Element>::iterator it = mainWin->getWorld()->begin();
  while(!(it->selected))
    it++;
  if(it->type == Element::TELEPORTER) {
    it->t->set_border(gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(sbborder)));
  }
  mainWin->update(EDITWINS);
  return TRUE;
}

gint chname(GtkWidget *enname, gpointer data) {
  MainWindow *mainWin = (MainWindow *) gtk_object_get_data(GTK_OBJECT(enname), "main_window");
  ListWindow &lw = mainWin->getListWindow();
  GtkWidget *list = lw.getList();
  vector<Element>::iterator it = mainWin->getWorld()->begin();
  while(!(it->selected))
    it++;
  it->name = gtk_entry_get_text(GTK_ENTRY(enname));
  mainWin->update(LIST);
  return TRUE;
}

gint showlist(GtkWidget *window, gpointer data) {
  MainWindow *mainWin = (MainWindow *) gtk_object_get_data(GTK_OBJECT(window), "main_window");
  mainWin->showList();
  return TRUE;
}

gint testw(GtkWidget *button, gpointer data) {
  // retrieve a temporary filename to write the world to
  char filename[256];
  strcpy(filename, "/tmp/bzedit-XXXXXX");
  mktemp(filename);
  cout << "Writing world file to " << filename << "...\n";
  return TRUE;
}

gint sellink(GtkCList *clist, gint row, gint column, GdkEventButton *event, gpointer data) {
  gtk_object_set_data(GTK_OBJECT(clist), "selection", (gpointer) (row + 1));
  return TRUE;
}

gint unsellink(GtkCList *clist, gint row, gint column, GdkEventButton *event, gpointer data) {
  gtk_object_set_data(GTK_OBJECT(clist), "selection", 0);
}

gint linkok(GtkWidget *button, Element *link) {
  GtkWidget *clist1 = GTK_WIDGET(gtk_object_get_data(GTK_OBJECT(button), "clist1"));
  GtkWidget *clist2 = GTK_WIDGET(gtk_object_get_data(GTK_OBJECT(button), "clist2"));
  GtkWidget *dialog = GTK_WIDGET(gtk_object_get_data(GTK_OBJECT(button), "dialog"));
  MainWindow *mainw = (MainWindow *) gtk_object_get_data(GTK_OBJECT(button), "mainw");
  char **text;
  int selection1 = (int) gtk_object_get_data(GTK_OBJECT(clist1), "selection") - 1;
  int selection2 = (int) gtk_object_get_data(GTK_OBJECT(clist2), "selection") - 1;
  string name1, name2;
  int side1, side2;
  if(selection1 == -1 || selection2 == -1) {
    GtkWidget *err = gnome_dialog_new("Error", GNOME_STOCK_BUTTON_OK, NULL);
    GtkWidget *errl = gtk_label_new("link attributes must be selected");
    gtk_widget_show(errl);
    gtk_box_pack_start(GTK_BOX(GNOME_DIALOG(err)->vbox), errl, TRUE, TRUE, 0);
    gnome_dialog_set_parent(GNOME_DIALOG(err), GTK_WINDOW(dialog));
    gnome_dialog_run_and_close(GNOME_DIALOG(err));
    return FALSE;
  }
  gtk_clist_get_text(GTK_CLIST(clist1), selection1, 0, text);
  name1 = text[0];
  gtk_clist_get_text(GTK_CLIST(clist1), selection1, 1, text);
  side1 = atoi(text[0]);
  gtk_clist_get_text(GTK_CLIST(clist2), selection2, 0, text);
  name2 = text[0];
  gtk_clist_get_text(GTK_CLIST(clist2), selection2, 1, text);
  side2 = atoi(text[0]);
  link->l->set_from(name1);
  link->l->set_from_side(side1);
  link->l->set_to(name2);
  link->l->set_to_side(side2);
  mainw->destroyLinkWin();
  return TRUE;
}

} // ns: UI::MW

} // nw: UI
