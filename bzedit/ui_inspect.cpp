#include "ui_inspect.h"
#include "globals.h"

namespace UI {

InspectorWindow::InspectorWindow() {
  GtkAdjustment *sa;
  
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Element Inspector");
  gtk_window_set_role(GTK_WINDOW(window), "inspector");
  gtk_window_set_type_hint(GTK_WINDOW(window), GDK_WINDOW_TYPE_HINT_DIALOG);
  g_object_set(G_OBJECT(window), "allow-grow", FALSE, NULL);
  gtk_signal_connect(GTK_OBJECT(window), "delete_event", GTK_SIGNAL_FUNC(CB::IW::deletee), NULL);

  // vbox to hold everything
  vbox = gtk_vbox_new(FALSE, 0);
  
  // hbox 1 - name, type
  hbox1 = gtk_hbox_new(FALSE, 0);
  label_name = gtk_label_new("Name");
  gtk_box_pack_start(GTK_BOX(hbox1), label_name, TRUE, FALSE, 0);
  entry_name = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(hbox1), entry_name, TRUE, TRUE, 0);
  label_type = gtk_label_new("Type");
  gtk_box_pack_start(GTK_BOX(hbox1), label_type, TRUE, FALSE, 0);
  entry_type = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(hbox1), entry_type, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), hbox1, FALSE, TRUE, 0);

  // main stuff
  table = gtk_table_new(2, 2, FALSE);
  frame_element_info = gtk_frame_new("Element Info");
  gtk_container_add(GTK_CONTAINER(frame_element_info), table);

  // all the little things
  frame_pos = gtk_frame_new("Position");
  tpos = gtk_table_new(3, 2, FALSE);
  labelX1 = gtk_label_new("X");
  gtk_table_attach_defaults(GTK_TABLE(tpos), labelX1, 0, 1, 0, 1);
  labelY1 = gtk_label_new("Y");
  gtk_table_attach_defaults(GTK_TABLE(tpos), labelY1, 0, 1, 1, 2);
  labelZ1 = gtk_label_new("Z");
  gtk_table_attach_defaults(GTK_TABLE(tpos), labelZ1, 0, 1, 2, 3);
  sa = (GtkAdjustment *) gtk_adjustment_new(0.0, -400.0, 400.0, 0.1, 1.0, 1.0);
  entryX1 = gtk_spin_button_new(sa, 0.1, 1);
  gtk_table_attach_defaults(GTK_TABLE(tpos), entryX1, 1, 2, 0, 1);
  sa = (GtkAdjustment *) gtk_adjustment_new(0.0, -400.0, 400.0, 0.1, 1.0, 1.0);
  entryY1 = gtk_spin_button_new(sa, 0.1, 1);
  gtk_table_attach_defaults(GTK_TABLE(tpos), entryY1, 1, 2, 1, 2);
  sa = (GtkAdjustment *) gtk_adjustment_new(0.0, 0.0, 1500.0, 0.1, 1.0, 1.0);
  entryZ1 = gtk_spin_button_new(sa, 0.1, 1);
  gtk_table_attach_defaults(GTK_TABLE(tpos), entryZ1, 1, 2, 2, 3);
  gtk_container_add(GTK_CONTAINER(frame_pos), tpos);
  gtk_table_attach_defaults(GTK_TABLE(table), frame_pos, 0, 1, 0, 1);
  
  frame_rotation = gtk_frame_new("Rotation");
  sa = (GtkAdjustment *) gtk_adjustment_new(0.0, 0.0, 360.0, 0.1, 1.0, 1.0);
  entryR = gtk_spin_button_new(sa, 0.1, 1);
  gtk_container_add(GTK_CONTAINER(frame_rotation), entryR);
  gtk_table_attach_defaults(GTK_TABLE(table), frame_rotation, 0, 1, 1, 2);
  
  frame_size = gtk_frame_new("Size");
  tsize = gtk_table_new(3, 2, FALSE);
  labelX2 = gtk_label_new("X");
  gtk_table_attach_defaults(GTK_TABLE(tsize), labelX2, 0, 1, 0, 1);
  labelY2 = gtk_label_new("Y");
  gtk_table_attach_defaults(GTK_TABLE(tsize), labelY2, 0, 1, 1, 2);
  labelZ2 = gtk_label_new("Z");
  gtk_table_attach_defaults(GTK_TABLE(tsize), labelZ2, 0, 1, 2, 3);
  sa = (GtkAdjustment *) gtk_adjustment_new(0.0, 0.0, 1200.0, 0.1, 1.0, 1.0);
  entryX2 = gtk_spin_button_new(sa, 0.1, 1);
  gtk_table_attach_defaults(GTK_TABLE(tsize), entryX2, 1, 2, 0, 1);
  sa = (GtkAdjustment *) gtk_adjustment_new(0.0, 0.0, 1200.0, 0.1, 1.0, 1.0);
  entryY2 = gtk_spin_button_new(sa, 0.1, 1);
  gtk_table_attach_defaults(GTK_TABLE(tsize), entryY2, 1, 2, 1, 2);
  sa = (GtkAdjustment *) gtk_adjustment_new(0.0, 0.0, 1200.0, 0.1, 1.0, 1.0);
  entryZ2 = gtk_spin_button_new(sa, 0.1, 1);
  gtk_table_attach_defaults(GTK_TABLE(tsize), entryZ2, 1, 2, 2, 3);
  gtk_container_add(GTK_CONTAINER(frame_size), tsize);
  gtk_table_attach_defaults(GTK_TABLE(table), frame_size, 1, 2, 0, 1);

  frame_other = gtk_frame_new("Other");
  tother = gtk_table_new(2, 2, FALSE);
  labelO1 = gtk_label_new("");
  gtk_table_attach_defaults(GTK_TABLE(tother), labelO1, 0, 1, 0, 1);
  labelO2 = gtk_label_new("");
  gtk_table_attach_defaults(GTK_TABLE(tother), labelO2, 0, 1, 1, 2);
  sa = (GtkAdjustment *) gtk_adjustment_new(0.0, 0.0, 1200.0, 1.0, 10.0, 10.0);
  entryO1 = gtk_spin_button_new(sa, 1.0, 1);
  gtk_table_attach_defaults(GTK_TABLE(tother), entryO1, 1, 2, 0, 1);
  sa = (GtkAdjustment *) gtk_adjustment_new(0.0, 0.0, 1200.0, 1.0, 10.0, 10.0);
  entryO2 = gtk_spin_button_new(sa, 1.0, 1);
  gtk_table_attach_defaults(GTK_TABLE(tother), entryO2, 1, 2, 1, 2);
  gtk_container_add(GTK_CONTAINER(frame_other), tother);
  gtk_table_attach_defaults(GTK_TABLE(table), frame_other, 1, 2, 1, 2);

  // put it in
  gtk_box_pack_start(GTK_BOX(vbox), frame_element_info, TRUE, TRUE, 0);
  
  // hbox 3 - buttons
  hbox3 = gtk_hbox_new(FALSE, 0);
  duplicate = gtk_button_new_with_label("Duplicate");
  gtk_signal_connect(GTK_OBJECT(duplicate), "clicked", GTK_SIGNAL_FUNC(CB::IW::duplicate), NULL);
  gtk_box_pack_start(GTK_BOX(hbox3), duplicate, TRUE, TRUE, 0);
  remove = gtk_button_new_with_label("Remove");
  gtk_signal_connect(GTK_OBJECT(remove), "clicked", GTK_SIGNAL_FUNC(CB::IW::remove), NULL);
  gtk_box_pack_start(GTK_BOX(hbox3), remove, TRUE, TRUE, 0);
  apply = gtk_button_new_with_label("Apply");
  gtk_signal_connect(GTK_OBJECT(apply), "clicked", GTK_SIGNAL_FUNC(CB::IW::apply), NULL);
  gtk_box_pack_start(GTK_BOX(hbox3), apply, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), hbox3, FALSE, TRUE, 0);

  gtk_container_add(GTK_CONTAINER(window), vbox);

  disable_all();
  
  gtk_widget_show_all(window);
}

void InspectorWindow::enable_name() {
  gtk_widget_set_sensitive(entry_name, TRUE);
}

void InspectorWindow::enable_type() {
  // should never be called
  gtk_widget_set_sensitive(entry_type, TRUE);
}

void InspectorWindow::enable_position_x() {
  gtk_widget_set_sensitive(entryX1, TRUE);
}

void InspectorWindow::enable_position_y() {
  gtk_widget_set_sensitive(entryY1, TRUE);
}

void InspectorWindow::enable_position_z() {
  gtk_widget_set_sensitive(entryZ1, TRUE);
}

void InspectorWindow::enable_rotation() {
  gtk_widget_set_sensitive(entryR, TRUE);
}

void InspectorWindow::enable_size_x() {
  gtk_widget_set_sensitive(entryX2, TRUE);
}

void InspectorWindow::enable_size_y() {
  gtk_widget_set_sensitive(entryY2, TRUE);
}

void InspectorWindow::enable_size_z() {
  gtk_widget_set_sensitive(entryZ2, TRUE);
}

void InspectorWindow::enable_other_1() {
  gtk_widget_set_sensitive(entryO1, TRUE);
}

void InspectorWindow::enable_other_2() {
  gtk_widget_set_sensitive(entryO2, TRUE);
}

void InspectorWindow::setboxpyr() {
  disable_all();
  enable_name();
  enable_position_x();
  enable_position_y();
  enable_position_z();
  enable_rotation();
  enable_size_x();
  enable_size_y();
  enable_size_z();
  gtk_widget_set_sensitive(duplicate, TRUE);
  gtk_widget_set_sensitive(remove, TRUE);
  gtk_widget_set_sensitive(apply, TRUE);
}

void InspectorWindow::settele() {
  disable_all();
  enable_name();
  enable_position_x();
  enable_position_y();
  enable_position_z();
  enable_rotation();
  enable_size_y();
  enable_size_z();
  gtk_label_set_text(GTK_LABEL(labelO1), "Border");
  GtkAdjustment *sa = (GtkAdjustment *) gtk_adjustment_new(0.0, 0.0, 20.0, 0.1, 1.0, 1.0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(entryO1), sa);
  enable_other_1();
  gtk_widget_set_sensitive(duplicate, TRUE);
  gtk_widget_set_sensitive(remove, TRUE);
  gtk_widget_set_sensitive(apply, TRUE);
}

void InspectorWindow::setlink() {
  disable_all();
  enable_name();
  gtk_label_set_text(GTK_LABEL(labelO1), "From side");
  gtk_label_set_text(GTK_LABEL(labelO2), "To side");
  GtkAdjustment *sa = (GtkAdjustment *) gtk_adjustment_new(0.0, -1.0, (doc->world.getNumOfType(TELEPORTER) * 2) - 1, 1.0, 1.0, 1.0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(entryO1), sa);
  sa = (GtkAdjustment *) gtk_adjustment_new(0.0, -1.0, (doc->world.getNumOfType(TELEPORTER) * 2) - 1, 1.0, 1.0, 1.0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(entryO2), sa);
  enable_other_1();
  enable_other_2();
  gtk_widget_set_sensitive(duplicate, TRUE);
  gtk_widget_set_sensitive(remove, TRUE);
  gtk_widget_set_sensitive(apply, TRUE);
}

void InspectorWindow::setbase() {
  disable_all();
  enable_name();
  enable_position_x();
  enable_position_y();
  enable_position_z();
  enable_rotation();
  enable_size_x();
  enable_size_y();
  gtk_label_set_text(GTK_LABEL(labelO1), "Color");
  GtkAdjustment *sa = (GtkAdjustment *) gtk_adjustment_new(0.0, 1.0, 4.0, 1.0, 1.0, 1.0);
  gtk_spin_button_set_adjustment(GTK_SPIN_BUTTON(entryO1), sa);
  enable_other_1();
  gtk_widget_set_sensitive(duplicate, TRUE);
  gtk_widget_set_sensitive(remove, TRUE);
  gtk_widget_set_sensitive(apply, TRUE);
}

void InspectorWindow::disable_all() {
  gtk_widget_set_sensitive(entryX1, FALSE);
  gtk_widget_set_sensitive(entryY1, FALSE);
  gtk_widget_set_sensitive(entryZ1, FALSE);
  gtk_widget_set_sensitive(entryR, FALSE);
  gtk_widget_set_sensitive(entryX2, FALSE);
  gtk_widget_set_sensitive(entryY2, FALSE);
  gtk_widget_set_sensitive(entryZ2, FALSE);
  gtk_label_set_text(GTK_LABEL(labelO1), "");
  gtk_label_set_text(GTK_LABEL(labelO2), "");
  gtk_widget_set_sensitive(entryO1, FALSE);
  gtk_widget_set_sensitive(entryO2, FALSE);
  gtk_widget_set_sensitive(entry_name, FALSE);
  gtk_widget_set_sensitive(entry_type, FALSE);
  gtk_entry_set_text(GTK_ENTRY(entryX1), "");
  gtk_entry_set_text(GTK_ENTRY(entryY1), "");
  gtk_entry_set_text(GTK_ENTRY(entryZ1), "");
  gtk_entry_set_text(GTK_ENTRY(entryR), "");
  gtk_entry_set_text(GTK_ENTRY(entryX2), "");
  gtk_entry_set_text(GTK_ENTRY(entryY2), "");
  gtk_entry_set_text(GTK_ENTRY(entryZ2), "");
  gtk_entry_set_text(GTK_ENTRY(entryO1), "");
  gtk_entry_set_text(GTK_ENTRY(entryO2), "");
  gtk_widget_set_sensitive(duplicate, FALSE);
  gtk_widget_set_sensitive(remove, FALSE);
  gtk_widget_set_sensitive(apply, FALSE);
}

void InspectorWindow::set_name(const char *contents) {
  gtk_entry_set_text(GTK_ENTRY(entry_name), contents);
}

void InspectorWindow::set_type(const char *contents) {
  gtk_entry_set_text(GTK_ENTRY(entry_type), contents);
}

void InspectorWindow::set_position_x(const char *contents) {
  gtk_entry_set_text(GTK_ENTRY(entryX1), contents);
}

void InspectorWindow::set_position_y(const char *contents) {
  gtk_entry_set_text(GTK_ENTRY(entryY1), contents);
}

void InspectorWindow::set_position_z(const char *contents) {
  gtk_entry_set_text(GTK_ENTRY(entryZ1), contents);
}

void InspectorWindow::set_rotation(const char *contents) {
  gtk_entry_set_text(GTK_ENTRY(entryR), contents);
}

void InspectorWindow::set_size_x(const char *contents) {
  gtk_entry_set_text(GTK_ENTRY(entryX2), contents);
}

void InspectorWindow::set_size_y(const char *contents) {
  gtk_entry_set_text(GTK_ENTRY(entryY2), contents);
}

void InspectorWindow::set_size_z(const char *contents) {
  gtk_entry_set_text(GTK_ENTRY(entryZ2), contents);
}

void InspectorWindow::set_other_1(const char *contents) {
  gtk_entry_set_text(GTK_ENTRY(entryO1), contents);
}

void InspectorWindow::set_other_2(const char *contents) {
  gtk_entry_set_text(GTK_ENTRY(entryO2), contents);
}

const char *InspectorWindow::get_name() {
  return gtk_entry_get_text(GTK_ENTRY(entry_name));
}

const char *InspectorWindow::get_position_x() {
  return gtk_entry_get_text(GTK_ENTRY(entryX1));
}

const char *InspectorWindow::get_position_y() {
  return gtk_entry_get_text(GTK_ENTRY(entryY1));
}

const char *InspectorWindow::get_position_z() {
  return gtk_entry_get_text(GTK_ENTRY(entryZ1));
}

const char *InspectorWindow::get_rotation() {
  return gtk_entry_get_text(GTK_ENTRY(entryR));
}

const char *InspectorWindow::get_size_x() {
  return gtk_entry_get_text(GTK_ENTRY(entryX2));
}

const char *InspectorWindow::get_size_y() {
  return gtk_entry_get_text(GTK_ENTRY(entryY2));
}

const char *InspectorWindow::get_size_z() {
  return gtk_entry_get_text(GTK_ENTRY(entryZ2));
}

const char *InspectorWindow::get_other_1() {
  return gtk_entry_get_text(GTK_ENTRY(entryO1));
}

const char *InspectorWindow::get_other_2() {
  return gtk_entry_get_text(GTK_ENTRY(entryO2));
}

GtkWidget *InspectorWindow::getWidget() {
  return window;
}

}
