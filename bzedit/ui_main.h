#ifndef __MAIN_WINDOW__
#define __MAIN_WINDOW__

#include <gnome.h>
#include <stl.h>
#include "ui_edit.h"
#include "ui_list.h"
#include "world.h"

namespace UI {

namespace MW {

const int SPINVALUES = 1 << 0;
const int SPINS      = 1 << 1;
const int EDITWINS   = 1 << 2;
const int LIST       = 1 << 3;

gint quit(GtkWidget *window, gpointer data);
gint newf(GtkWidget *window, gpointer data);
gint neww(GtkWidget *window, gpointer data);
gint open(GtkWidget *window, gpointer data);
gint openok(GtkWidget *button, GtkFileSelection *fsel);
gint save(GtkWidget *window, gpointer data);
gint saveas(GtkWidget *window, gpointer data);
gint saveasok(GtkWidget *button, GtkFileSelection *fsel);
gint import(GtkWidget *window, gpointer data);
gint importok(GtkWidget *button, GtkFileSelection *fsel);
gint abox(GtkWidget *window, gpointer data);
gint apyr(GtkWidget *window, gpointer data);
gint atel(GtkWidget *window, gpointer data);
gint alnk(GtkWidget *window, gpointer data);
gint undo(GtkWidget *window, gpointer data);
gint redo(GtkWidget *window, gpointer data);
gint cut(GtkWidget *window, gpointer data);
gint copy(GtkWidget *window, gpointer data);
gint paste(GtkWidget *window, gpointer data);
gint clear(GtkWidget *window, gpointer data);
gint selall(GtkWidget *window, gpointer data);
gint about(GtkWidget *window, gpointer data);
gint chpx(GtkWidget *sbpx, gpointer data);
gint chpy(GtkWidget *sbpy, gpointer data);
gint chpz(GtkWidget *sbpz, gpointer data);
gint changle(GtkWidget *sbangle, gpointer data);
gint chsx(GtkWidget *sbsx, gpointer data);
gint chsy(GtkWidget *sbsy, gpointer data);
gint chsz(GtkWidget *sbsz, gpointer data);
gint chborder(GtkWidget *sbborder, gpointer data);
gint chname(GtkWidget *enname, gpointer data);
gint showlist(GtkWidget *window, gpointer data);
gint testw(GtkWidget *button, gpointer data);
gint sellink(GtkCList *clist, gint row, gint column, GdkEventButton *event, gpointer data);
gint unsellink(GtkCList *clist, gint row, gint column, GdkEventButton *event, gpointer data);
gint linkok(GtkWidget *button, Element *link);

static GnomeUIInfo file_menu[] = {
  GNOMEUIINFO_MENU_NEW_WINDOW_ITEM(MW::neww, NULL),
  GNOMEUIINFO_SEPARATOR,
  GNOMEUIINFO_MENU_NEW_ITEM("New", "Create a new BZFlag world", MW::newf, NULL),
  GNOMEUIINFO_MENU_OPEN_ITEM(MW::open, NULL),
  GNOMEUIINFO_ITEM_STOCK("Import", "Import another world into the current one", MW::import, GNOME_STOCK_MENU_JUMP_TO),
  GNOMEUIINFO_MENU_SAVE_ITEM(MW::save, NULL),
  GNOMEUIINFO_MENU_SAVE_AS_ITEM(MW::saveas, NULL),
  GNOMEUIINFO_SEPARATOR,
  GNOMEUIINFO_MENU_EXIT_ITEM(MW::quit, NULL),
  GNOMEUIINFO_END
};
static GnomeUIInfo eAdd_menu[] = {
  GNOMEUIINFO_ITEM_NONE("Box", "Add a box to the current world", MW::abox),
  GNOMEUIINFO_ITEM_NONE("Pyramid", "Add a pyramid to the current world", MW::apyr),
  GNOMEUIINFO_ITEM_NONE("Teleporter", "Add a teleporter to the current world", MW::atel),
  GNOMEUIINFO_ITEM_NONE("Link", "Add a link to the current world", MW::alnk),
  GNOMEUIINFO_END
};
static GnomeUIInfo edit_menu[] = {
  GNOMEUIINFO_MENU_UNDO_ITEM(MW::undo, NULL),
  GNOMEUIINFO_MENU_REDO_ITEM(MW::redo, NULL),
  GNOMEUIINFO_SEPARATOR,
  GNOMEUIINFO_MENU_CUT_ITEM(MW::cut, NULL),
  GNOMEUIINFO_MENU_COPY_ITEM(MW::copy, NULL),
  GNOMEUIINFO_MENU_PASTE_ITEM(MW::paste, NULL),
  GNOMEUIINFO_MENU_CLEAR_ITEM(MW::clear, NULL),
  GNOMEUIINFO_MENU_SELECT_ALL_ITEM(MW::selall, NULL),
  GNOMEUIINFO_SEPARATOR,
  GNOMEUIINFO_SUBTREE_STOCK("Add", eAdd_menu, GNOME_STOCK_MENU_LINE_IN),
  GNOMEUIINFO_END
};
static GnomeUIInfo windows_menu[] = {
  GNOMEUIINFO_ITEM_STOCK("List Window", "Show the list window", MW::showlist, GNOME_STOCK_MENU_LINE_IN),
  GNOMEUIINFO_END
};
static GnomeUIInfo help_menu[] = {
  GNOMEUIINFO_MENU_ABOUT_ITEM(MW::about, NULL),
  GNOMEUIINFO_SEPARATOR,
  GNOMEUIINFO_HELP((void *) "bzedit"),
  GNOMEUIINFO_END
};
static GnomeUIInfo main_menu[] = {
  GNOMEUIINFO_MENU_FILE_TREE(file_menu),
  GNOMEUIINFO_MENU_EDIT_TREE(edit_menu),
  GNOMEUIINFO_MENU_WINDOWS_TREE(windows_menu),
  GNOMEUIINFO_MENU_HELP_TREE(help_menu),
  GNOMEUIINFO_END
};

const int SIZE_X = 1 << 0;
const int SIZE_Y = 1 << 1;
const int SIZE_Z = 1 << 2;

}

class MainWindow {
  public:
    MainWindow();
    ~MainWindow();

    void enable_position();
    void enable_rotation();
    void enable_size();
    void enable_border();
    void enable_size(int which);
    void enable_name();
    void enable_most();				// All but border
    void enable_tele();				// +border, -sy
    void disable_position();
    void disable_rotation();
    void disable_size();
    void disable_border();
    void disable_size(int which);
    void disable_name();
    void disable_all();
    void update(short updates);			// updates all windows
    void addEW(EditWindow *ew);
    void showList();
    void setWorld(World *world);
    void showLinkWin(Element *link);
    void destroyLinkWin();
    World *getWorld();
    GtkWidget *getWindow();
    ListWindow &getListWindow();
  protected:
    GtkWidget *window;
    GtkWidget *statusbar;
    GtkWidget *table;
    GtkWidget *lpos, *lrot, *lsize, *lbor, *lname;
    GtkWidget *sbpx, *sbpy, *sbpz;
    GtkWidget *sbangle;
    GtkWidget *sbsx, *sbsy, *sbsz;
    GtkWidget *sbborder;
    GtkWidget *enname;
    GtkWidget *testworld;
    GtkWidget *linkwin;
    GtkAdjustment *sa;
    vector<EditWindow *> edits;
    ListWindow lw;
    World *world;
};

}

#endif
