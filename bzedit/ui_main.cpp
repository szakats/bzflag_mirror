#include "ui_main.h"
#include "globals.h"
#include <iostream>

namespace UI {

MainWindow::MainWindow() {
  int glargs[] = {
    GDK_GL_RGBA,
    GDK_GL_DOUBLEBUFFER,
    GDK_GL_DEPTH_SIZE, 1,
    GDK_GL_NONE};
  
  // create the window
  window = gnome_app_new("BZEdit", "BZFlag Editor");
  gtk_signal_connect(GTK_OBJECT(window), "delete_event", GTK_SIGNAL_FUNC(CB::MW::exit), NULL);
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
  gtk_window_set_role(GTK_WINDOW(window), "main");

  // first check that GL works
  if(gdk_gl_query() == FALSE)
    g_print("OpenGL not supported!\n");

  // create menus, toolbar, statusbar
  statusbar = gnome_appbar_new(FALSE, TRUE, GNOME_PREFERENCES_USER);
  gnome_app_create_menus(GNOME_APP(window), main_menu);
  gnome_app_set_statusbar(GNOME_APP(window), statusbar);
  gnome_app_install_menu_hints(GNOME_APP(window), main_menu);
  toolbar = create_main_toolbar(window, &main_toolbar);
  gnome_app_add_toolbar(GNOME_APP(window), GTK_TOOLBAR(toolbar), "main.toolbar", BONOBO_DOCK_ITEM_BEH_EXCLUSIVE, BONOBO_DOCK_TOP, 1, 0, 0);

  // create the gl area
  if((glarea = gtk_gl_area_new(glargs)) == NULL)
    g_print("Error creating GtkGLArea!\n");
  gtk_widget_set_events(GTK_WIDGET(glarea), GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK | GDK_BUTTON3_MOTION_MASK | GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK | GDK_SCROLL_MASK);
  gtk_signal_connect(GTK_OBJECT(glarea), "realize", GTK_SIGNAL_FUNC(CB::MW::initialize), NULL);
  // allow the gl area to grab the focus, and grab it
  GTK_WIDGET_SET_FLAGS(glarea, GTK_CAN_FOCUS);
  gtk_widget_grab_focus(GTK_WIDGET(glarea));

  // pack
  gnome_app_set_contents(GNOME_APP(window), glarea);

  // show the main window
  gtk_widget_show_all(window);

  // create the other windows
  list = new ListWindow;
  inspect = new InspectorWindow;

  // toggle the menus
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(UI::view_menu[2].widget), TRUE);
  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(UI::view_menu[3].widget), TRUE);
  showlist = true;
  showinspector = true;

  kshift = false; kcontrol = false;
  selected = -1;
  transparency = 100.0;
}

void MainWindow::setsignals() {
  gtk_signal_connect(GTK_OBJECT(glarea), "expose_event", GTK_SIGNAL_FUNC(CB::MW::draw), NULL);
  gtk_signal_connect(GTK_OBJECT(glarea), "configure_event", GTK_SIGNAL_FUNC(CB::MW::reshape), NULL);
  gtk_signal_connect(GTK_OBJECT(glarea), "button_press_event", GTK_SIGNAL_FUNC(CB::MW::buttonpress), NULL);
  gtk_signal_connect(GTK_OBJECT(glarea), "motion_notify_event", GTK_SIGNAL_FUNC(CB::MW::mousemotion), NULL);
  gtk_signal_connect(GTK_OBJECT(glarea), "key_press_event", GTK_SIGNAL_FUNC(CB::MW::keypress), NULL);
  gtk_signal_connect(GTK_OBJECT(glarea), "key_release_event", GTK_SIGNAL_FUNC(CB::MW::keyrelease), NULL);
  gtk_signal_connect(GTK_OBJECT(glarea), "scroll_event", GTK_SIGNAL_FUNC(CB::MW::scroll), NULL);
}

void MainWindow::redraw() {
  if(gtk_gl_area_make_current(GTK_GL_AREA(glarea))) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(doc != NULL) {
      doc->camera.setup();
      doc->world.render(transparency);
    }
    gtk_gl_area_swapbuffers(GTK_GL_AREA(glarea));
  } else {
    // could not set context
  }
}

void MainWindow::unselectAll() {
  selected = -1;
  for(unsigned int i = 0; i < doc->world.list.size(); i++)
    doc->world.list[i].selected = false;
  list->unselectAll();
  inspect->disable_all();
  redraw();
}

void MainWindow::select(int item) {
  selected = item;
  for(unsigned int i = 0; i < doc->world.list.size(); i++)
    doc->world.list[i].selected = false;
  doc->world.list[item].selected = true;
  list->setView(ALL);
  list->selectItem(doc->world.list[item]);
  char buffer[255];
  if(doc->world.list[item].type == BOX) {
    inspect->setboxpyr();
    inspect->set_name(doc->world.list[item].name.c_str());
    inspect->set_type("Box");
  } else if(doc->world.list[item].type == PYRAMID) {
    inspect->setboxpyr();
    inspect->set_name(doc->world.list[item].name.c_str());
    inspect->set_type("Pyramid");
  } else if(doc->world.list[item].type == TELEPORTER) {
    inspect->settele();
    inspect->set_name(doc->world.list[item].name.c_str());
    inspect->set_type("Teleporter");
    float num;
    doc->world.list[item].obj->getItemInfo("border", num);
    sprintf(buffer, "%f", num);
    inspect->set_other_1(buffer);
  } else if(doc->world.list[item].type == LINK) {
    inspect->setlink();
    inspect->set_name(doc->world.list[item].name.c_str());
    inspect->set_type("Link");
    int num;
    doc->world.list[item].obj->getItemInfo("fromside", num);
    sprintf(buffer, "%i", num);
    inspect->set_other_1(buffer);
    doc->world.list[item].obj->getItemInfo("toside", num);
    sprintf(buffer, "%i", num);
    inspect->set_other_2(buffer);
  } else if(doc->world.list[item].type == BASE) {
    inspect->setbase();
    inspect->set_name(doc->world.list[item].name.c_str());
    inspect->set_type("Base");
    int num;
    doc->world.list[item].obj->getItemInfo("color", num);
    sprintf(buffer, "%i", num);
    inspect->set_other_1(buffer);
  }
  if(doc->world.list[item].type == BOX || doc->world.list[item].type == PYRAMID || doc->world.list[item].type == TELEPORTER ||  doc->world.list[item].type == BASE) {
    sprintf(buffer, "%f", doc->world.list[item].obj->get_px());
    inspect->set_position_x(buffer);
    sprintf(buffer, "%f", doc->world.list[item].obj->get_py());
    inspect->set_position_y(buffer);
    sprintf(buffer, "%f", doc->world.list[item].obj->get_pz());
    inspect->set_position_z(buffer);
    sprintf(buffer, "%f", doc->world.list[item].obj->get_angle());
    inspect->set_rotation(buffer);
    sprintf(buffer, "%f", doc->world.list[item].obj->get_sy());
    inspect->set_size_y(buffer);
  }
  if(doc->world.list[item].type == BOX || doc->world.list[item].type == PYRAMID || doc->world.list[item].type == BASE) {
    sprintf(buffer, "%f", doc->world.list[item].obj->get_sx());
    inspect->set_size_x(buffer);
  }
  if(doc->world.list[item].type == BOX || doc->world.list[item].type == PYRAMID || doc->world.list[item].type == TELEPORTER) {
    sprintf(buffer, "%f", doc->world.list[item].obj->get_sz());
    inspect->set_size_z(buffer);
  }
  redraw();
}

void MainWindow::select(std::string item) {
  int itemnum = doc->world.findElementWithName(item);
  if(itemnum == -1)
    return;
  select(itemnum);
}

void MainWindow::duplicateselected() {
  Element *ele = doc->world.getElement(selected);
  if(!ele)
    return;
  int copy = doc->world.addElement(ele->type);
  Element *copye = doc->world.getElement(copy);
  if(!copye)
    return;
  copye->obj->copyFrom(ele->obj);
  std::string temp = ele->name.c_str();
  temp += " copy";
  copye->name = temp;
  mainwin->list->addItem(*copye);
  mainwin->select(copy);
  mainwin->redraw();
}

void MainWindow::removeselected() {
  list->removeItem(*(doc->world.getElement(selected)));
  doc->world.removeElement(selected);
  unselectAll();
}

void MainWindow::applychanges() {
  const char *name = inspect->get_name();
  doc->world.list[selected].name = name;
  if(doc->world.list[selected].type == TELEPORTER) {
    float num;
    num = atof(inspect->get_other_1());
    doc->world.list[selected].obj->setItemInfo("border", num);
  } else if(doc->world.list[selected].type == LINK) {
    int num;
    num = atoi(inspect->get_other_1());
    doc->world.list[selected].obj->setItemInfo("fromside", num);
    num = atoi(inspect->get_other_2());
    doc->world.list[selected].obj->setItemInfo("toside", num);
  } else if(doc->world.list[selected].type == BASE) {
    int num;
    num = atoi(inspect->get_other_1());
    doc->world.list[selected].obj->setItemInfo("color", num);
  }
  if(doc->world.list[selected].type == BOX || doc->world.list[selected].type == PYRAMID || doc->world.list[selected].type == TELEPORTER ||  doc->world.list[selected].type == BASE) {
    float num;
    num = atof(inspect->get_position_x());
    doc->world.list[selected].obj->set_px(num);
    num = atof(inspect->get_position_y());
    doc->world.list[selected].obj->set_py(num);
    num = atof(inspect->get_position_z());
    doc->world.list[selected].obj->set_pz(num);
    num = atof(inspect->get_rotation());
    doc->world.list[selected].obj->set_angle(num);
    num = atof(inspect->get_size_y());
    doc->world.list[selected].obj->set_sy(num);
  }
  if(doc->world.list[selected].type == BOX || doc->world.list[selected].type == PYRAMID || doc->world.list[selected].type == BASE) {
    float num;
    num = atof(inspect->get_size_x());
    doc->world.list[selected].obj->set_sx(num);
  }
  if(doc->world.list[selected].type == BOX || doc->world.list[selected].type == PYRAMID || doc->world.list[selected].type == TELEPORTER) {
    float num;
    num = atof(inspect->get_size_z());
    doc->world.list[selected].obj->set_sz(num);
  }
  doc->world.list[selected].obj->init();
  list->selectItem(doc->world.list[selected]);
  redraw();
}

void MainWindow::movetoselected() {
  doc->camera.fx = doc->world.list[selected].obj->get_px();
  doc->camera.fy = doc->world.list[selected].obj->get_py();
  doc->camera.fz = doc->world.list[selected].obj->get_pz();
  doc->camera.fcamera = true;
  redraw();
}

void MainWindow::updatetrans() {
  GtkAdjustment *adj = gtk_range_get_adjustment(GTK_RANGE(main_toolbar.slider));
  transparency = gtk_adjustment_get_value(adj);
  redraw();
}

GtkWidget *MainWindow::getWidget() {
  return window;
}

void MainWindow::toggle_list() {
  if(showlist) {
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(UI::view_menu[2].widget), FALSE);
    gtk_widget_hide(list->getWidget());
  } else {
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(UI::view_menu[2].widget), TRUE);
    gtk_widget_show(list->getWidget());
  }
  showlist = !showlist;
}

void MainWindow::toggle_inspector() {
  if(showinspector) {
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(UI::view_menu[3].widget), FALSE);
    gtk_widget_hide(inspect->getWidget());
  } else {
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(UI::view_menu[3].widget), TRUE);
    gtk_widget_show(inspect->getWidget());
  }
  showinspector = !showinspector;
}

}
