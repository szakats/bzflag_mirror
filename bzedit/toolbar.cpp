#include "toolbar.h"
namespace UI {

GtkWidget *create_main_toolbar(GtkWidget *parent, MainToolbar *toolbar) {
  GtkWidget *toolbar1;
  GtkWidget *toolbar_newf;
  GtkWidget *toolbar_open;
  GtkWidget *toolbar_save;
  GtkWidget *toolbar_zplus;
  GtkWidget *toolbar_yplus;
  GtkWidget *toolbar_yminus;
  GtkWidget *toolbar_xplus;
  GtkWidget *toolbar_xminus;
  GtkWidget *toolbar_iso;
  GtkWidget *toolbar_target;
  GtkWidget *toolbar_abox;
  GtkWidget *toolbar_apyr;
  GtkWidget *toolbar_atel;
  GtkWidget *toolbar_alnk;
  GtkWidget *toolbar_abas;
  GtkWidget *toolbar_slider;

  GtkTooltips *tooltips;
  GtkWidget *tmp_toolbar_icon;
  GtkAdjustment *sa;

  tooltips = gtk_tooltips_new();

  toolbar1 = gtk_toolbar_new();
  gtk_toolbar_set_orientation(GTK_TOOLBAR(toolbar1), GTK_ORIENTATION_HORIZONTAL);
  gtk_widget_ref(toolbar1);
  gtk_widget_show(toolbar1);

  tmp_toolbar_icon = gtk_image_new_from_stock(GTK_STOCK_NEW, GTK_ICON_SIZE_SMALL_TOOLBAR);
  toolbar_newf = gtk_toolbar_append_element(GTK_TOOLBAR(toolbar1), GTK_TOOLBAR_CHILD_BUTTON, NULL, NULL, _("Create a new BZFlag world"), NULL, tmp_toolbar_icon, NULL, NULL);
  gtk_widget_ref(toolbar_newf);
  gtk_widget_show(toolbar_newf);

  tmp_toolbar_icon = gtk_image_new_from_stock(GTK_STOCK_OPEN, GTK_ICON_SIZE_SMALL_TOOLBAR);
  toolbar_open = gtk_toolbar_append_element(GTK_TOOLBAR(toolbar1), GTK_TOOLBAR_CHILD_BUTTON, NULL, NULL, _("Open a BZFlag world"), NULL, tmp_toolbar_icon, NULL, NULL);
  gtk_widget_ref(toolbar_open);
  gtk_widget_show(toolbar_open);

  tmp_toolbar_icon = gtk_image_new_from_stock(GTK_STOCK_SAVE, GTK_ICON_SIZE_SMALL_TOOLBAR);
  toolbar_save = gtk_toolbar_append_element(GTK_TOOLBAR(toolbar1), GTK_TOOLBAR_CHILD_BUTTON, NULL, NULL, _("Save the current world"), NULL, tmp_toolbar_icon, NULL, NULL);
  gtk_widget_ref(toolbar_save);
  gtk_widget_show(toolbar_save);

  gtk_toolbar_append_space(GTK_TOOLBAR(toolbar1));

  tmp_toolbar_icon = gnome_pixmap_new_from_xpm_d(TB::zplus_xpm);
  toolbar_zplus = gtk_toolbar_append_element(GTK_TOOLBAR(toolbar1), GTK_TOOLBAR_CHILD_BUTTON, NULL, NULL, _("View from above"), NULL, tmp_toolbar_icon, NULL, NULL);
  gtk_widget_ref(toolbar_zplus);
  gtk_widget_show(toolbar_zplus);

  gtk_toolbar_append_space(GTK_TOOLBAR(toolbar1));

  tmp_toolbar_icon = gnome_pixmap_new_from_xpm_d(TB::yplus_xpm);
  toolbar_yplus = gtk_toolbar_append_element(GTK_TOOLBAR(toolbar1), GTK_TOOLBAR_CHILD_BUTTON, NULL, NULL, _("View from +Y"), NULL, tmp_toolbar_icon, NULL, NULL);
  gtk_widget_ref(toolbar_yplus);
  gtk_widget_show(toolbar_yplus);

  tmp_toolbar_icon = gnome_pixmap_new_from_xpm_d(TB::yminus_xpm);
  toolbar_yminus = gtk_toolbar_append_element(GTK_TOOLBAR(toolbar1), GTK_TOOLBAR_CHILD_BUTTON, NULL, NULL, _("View from -Y"), NULL, tmp_toolbar_icon, NULL, NULL);
  gtk_widget_ref(toolbar_yminus);
  gtk_widget_show(toolbar_yminus);

  gtk_toolbar_append_space(GTK_TOOLBAR(toolbar1));

  tmp_toolbar_icon = gnome_pixmap_new_from_xpm_d(TB::xplus_xpm);
  toolbar_xplus = gtk_toolbar_append_element(GTK_TOOLBAR(toolbar1), GTK_TOOLBAR_CHILD_BUTTON, NULL, NULL, _("View from +X"), NULL, tmp_toolbar_icon, NULL, NULL);
  gtk_widget_ref(toolbar_xplus);
  gtk_widget_show(toolbar_xplus);

  tmp_toolbar_icon = gnome_pixmap_new_from_xpm_d(TB::xminus_xpm);
  toolbar_xminus = gtk_toolbar_append_element(GTK_TOOLBAR(toolbar1), GTK_TOOLBAR_CHILD_BUTTON, NULL, NULL, _("View from -X"), NULL, tmp_toolbar_icon, NULL, NULL);
  gtk_widget_ref(toolbar_xminus);
  gtk_widget_show(toolbar_xminus);

  gtk_toolbar_append_space(GTK_TOOLBAR(toolbar1));

  tmp_toolbar_icon = gnome_pixmap_new_from_xpm_d(TB::iso_xpm);
  toolbar_iso = gtk_toolbar_append_element(GTK_TOOLBAR(toolbar1), GTK_TOOLBAR_CHILD_BUTTON, NULL, NULL, _("Isometric view"), NULL, tmp_toolbar_icon, NULL, NULL);
  gtk_widget_ref(toolbar_iso);
  gtk_widget_show(toolbar_iso);

  gtk_toolbar_append_space(GTK_TOOLBAR(toolbar1));

  tmp_toolbar_icon = gnome_pixmap_new_from_xpm_d(TB::target_xpm);
  toolbar_target = gtk_toolbar_append_element(GTK_TOOLBAR(toolbar1), GTK_TOOLBAR_CHILD_BUTTON, NULL, NULL, _("Center view on (0, 0, 0)"), NULL, tmp_toolbar_icon, NULL, NULL);
  gtk_widget_ref(toolbar_target);
  gtk_widget_show(toolbar_target);

  gtk_toolbar_append_space(GTK_TOOLBAR(toolbar1));

  tmp_toolbar_icon = gnome_pixmap_new_from_xpm_d(TB::addbox_xpm);
  toolbar_abox = gtk_toolbar_append_element(GTK_TOOLBAR(toolbar1), GTK_TOOLBAR_CHILD_BUTTON, NULL, NULL, _("Add a box"), NULL, tmp_toolbar_icon, NULL, NULL);
  gtk_widget_ref(toolbar_abox);
  gtk_widget_show(toolbar_abox);

  tmp_toolbar_icon = gnome_pixmap_new_from_xpm_d(TB::addpyr_xpm);
  toolbar_apyr = gtk_toolbar_append_element(GTK_TOOLBAR(toolbar1), GTK_TOOLBAR_CHILD_BUTTON, NULL, NULL, _("Add a pyramid"), NULL, tmp_toolbar_icon, NULL, NULL);
  gtk_widget_ref(toolbar_apyr);
  gtk_widget_show(toolbar_apyr);

  tmp_toolbar_icon = gnome_pixmap_new_from_xpm_d(TB::addtele_xpm);
  toolbar_atel = gtk_toolbar_append_element(GTK_TOOLBAR(toolbar1), GTK_TOOLBAR_CHILD_BUTTON, NULL, NULL, _("Add a teleporter"), NULL, tmp_toolbar_icon, NULL, NULL);
  gtk_widget_ref(toolbar_atel);
  gtk_widget_show(toolbar_atel);
 
  tmp_toolbar_icon = gnome_pixmap_new_from_xpm_d(TB::addlink_xpm);
  toolbar_alnk = gtk_toolbar_append_element(GTK_TOOLBAR(toolbar1), GTK_TOOLBAR_CHILD_BUTTON, NULL, NULL, _("Add a link"), NULL, tmp_toolbar_icon, NULL, NULL);
  gtk_widget_ref(toolbar_alnk);
  gtk_widget_show(toolbar_alnk);
 
  tmp_toolbar_icon = gnome_pixmap_new_from_xpm_d(TB::addbase_xpm);
  toolbar_abas = gtk_toolbar_append_element(GTK_TOOLBAR(toolbar1), GTK_TOOLBAR_CHILD_BUTTON, NULL, NULL, _("Add a base"), NULL, tmp_toolbar_icon, NULL, NULL);
  gtk_widget_ref(toolbar_abas);
  gtk_widget_show(toolbar_abas);

  sa = GTK_ADJUSTMENT(gtk_adjustment_new(100.0, 0.0, 100.0, 1.0, 10.0, 0.0));
  toolbar_slider = gtk_hscale_new(sa);
  gtk_widget_set_size_request(toolbar_slider, 75, -1);
  gtk_scale_set_draw_value(GTK_SCALE(toolbar_slider), FALSE);
  gtk_toolbar_append_widget(GTK_TOOLBAR(toolbar1), toolbar_slider, NULL, NULL);
  gtk_tooltips_set_tip(GTK_TOOLTIPS(tooltips), toolbar_slider, _("Transparency"), NULL);
  gtk_widget_ref(toolbar_slider);
  gtk_widget_show(toolbar_slider);

  gtk_signal_connect(GTK_OBJECT(toolbar_newf), "clicked", GTK_SIGNAL_FUNC(CB::MW::newf), NULL);
  gtk_signal_connect(GTK_OBJECT(toolbar_open), "clicked", GTK_SIGNAL_FUNC(CB::MW::open), NULL);
  gtk_signal_connect(GTK_OBJECT(toolbar_save), "clicked", GTK_SIGNAL_FUNC(CB::MW::save), NULL);
  gtk_signal_connect(GTK_OBJECT(toolbar_zplus), "clicked", GTK_SIGNAL_FUNC(CB::MW::tb_zplus), NULL);
  gtk_signal_connect(GTK_OBJECT(toolbar_yplus), "clicked", GTK_SIGNAL_FUNC(CB::MW::tb_yplus), NULL);
  gtk_signal_connect(GTK_OBJECT(toolbar_yminus), "clicked", GTK_SIGNAL_FUNC(CB::MW::tb_yminus), NULL);
  gtk_signal_connect(GTK_OBJECT(toolbar_xplus), "clicked", GTK_SIGNAL_FUNC(CB::MW::tb_xplus), NULL);
  gtk_signal_connect(GTK_OBJECT(toolbar_xminus), "clicked", GTK_SIGNAL_FUNC(CB::MW::tb_xminus), NULL);
  gtk_signal_connect(GTK_OBJECT(toolbar_iso), "clicked", GTK_SIGNAL_FUNC(CB::MW::tb_iso), NULL);
  gtk_signal_connect(GTK_OBJECT(toolbar_target), "clicked", GTK_SIGNAL_FUNC(CB::MW::tb_target), NULL);
  gtk_signal_connect(GTK_OBJECT(toolbar_abox), "clicked", GTK_SIGNAL_FUNC(CB::MW::tb_addbox), NULL);
  gtk_signal_connect(GTK_OBJECT(toolbar_apyr), "clicked", GTK_SIGNAL_FUNC(CB::MW::tb_addpyr), NULL);
  gtk_signal_connect(GTK_OBJECT(toolbar_atel), "clicked", GTK_SIGNAL_FUNC(CB::MW::tb_addtel), NULL);
  gtk_signal_connect(GTK_OBJECT(toolbar_alnk), "clicked", GTK_SIGNAL_FUNC(CB::MW::tb_addlnk), NULL);
  gtk_signal_connect(GTK_OBJECT(toolbar_abas), "clicked", GTK_SIGNAL_FUNC(CB::MW::tb_addbas), NULL);
  gtk_signal_connect(GTK_OBJECT(sa), "value_changed", GTK_SIGNAL_FUNC(CB::MW::tb_trans), NULL);
  gtk_signal_connect(GTK_OBJECT(sa), "changed", GTK_SIGNAL_FUNC(CB::MW::tb_trans), NULL);

  toolbar->toolbar = toolbar1;
  toolbar->newf = toolbar_newf;
  toolbar->open = toolbar_open;
  toolbar->save = toolbar_save;
  toolbar->zplus = toolbar_zplus;
  toolbar->yplus = toolbar_yplus;
  toolbar->yminus = toolbar_yminus;
  toolbar->xplus = toolbar_xplus;
  toolbar->xminus = toolbar_xminus;
  toolbar->iso = toolbar_iso;
  toolbar->target = toolbar_target;
  toolbar->abox = toolbar_abox;
  toolbar->apyr = toolbar_apyr;
  toolbar->atel = toolbar_atel;
  toolbar->alnk = toolbar_alnk;
  toolbar->abas = toolbar_abas;
  toolbar->slider = toolbar_slider;

  return toolbar1;
}

}
