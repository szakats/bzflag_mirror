#ifndef __TOOLBAR_H__
#define __TOOLBAR_H__

#include <gnome.h>
#include "callbacks.h"

namespace UI {

namespace TB {
// xpm image data
#include "data/stub.xpm"
#include "data/addbase.xpm"
#include "data/addbox.xpm"
#include "data/addlink.xpm"
#include "data/addpyr.xpm"
#include "data/addtele.xpm"
#include "data/iso.xpm"
#include "data/target.xpm"
#include "data/xminus.xpm"
#include "data/xplus.xpm"
#include "data/yminus.xpm"
#include "data/yplus.xpm"
#include "data/zplus.xpm"
}

struct MainToolbar {
  GtkWidget *toolbar;
  GtkWidget *newf;
  GtkWidget *open;
  GtkWidget *save;
  GtkWidget *zplus;
  GtkWidget *yplus;
  GtkWidget *yminus;
  GtkWidget *xplus;
  GtkWidget *xminus;
  GtkWidget *iso;
  GtkWidget *target;
  GtkWidget *abox;
  GtkWidget *apyr;
  GtkWidget *atel;
  GtkWidget *alnk;
  GtkWidget *abas;
  GtkWidget *slider;
};

GtkWidget *create_main_toolbar(GtkWidget *parent, MainToolbar *toolbar);

}

#endif
