#ifndef __MENUS_H__
#define __MENUS_H__

#include <gnome.h>
#include "callbacks.h"

namespace UI {

static GnomeUIInfo file_menu[] = {
  GNOMEUIINFO_MENU_NEW_ITEM("New", "Create a new BZFlag world", CB::MW::newf, NULL),
  GNOMEUIINFO_MENU_OPEN_ITEM(CB::MW::open, NULL),
  GNOMEUIINFO_MENU_SAVE_ITEM(CB::MW::save, NULL),
  GNOMEUIINFO_MENU_SAVE_AS_ITEM(CB::MW::saveas, NULL),
  GNOMEUIINFO_SEPARATOR,
  GNOMEUIINFO_MENU_EXIT_ITEM(CB::MW::exit, NULL),
  GNOMEUIINFO_END
};

static GnomeUIInfo edit_menu[] = {
  GNOMEUIINFO_MENU_UNDO_ITEM(CB::MW::undo, NULL),
  GNOMEUIINFO_MENU_REDO_ITEM(CB::MW::redo, NULL),
  GNOMEUIINFO_SEPARATOR,
  GNOMEUIINFO_MENU_CUT_ITEM(CB::MW::cut, NULL),
  GNOMEUIINFO_MENU_COPY_ITEM(CB::MW::copy, NULL),
  GNOMEUIINFO_MENU_PASTE_ITEM(CB::MW::paste, NULL),
  GNOMEUIINFO_END
};

static GnomeUIInfo view_menu[] = {
  GNOMEUIINFO_TOGGLEITEM("Grid", "Show the grid", CB::MW::vgrid, NULL),
  GNOMEUIINFO_SEPARATOR,
  GNOMEUIINFO_TOGGLEITEM("Element List", "Show the list of elements", CB::MW::vlist, NULL),
  GNOMEUIINFO_TOGGLEITEM("Element Inspector", "Show the element inspector", CB::MW::vinspect, NULL),
  GNOMEUIINFO_SEPARATOR,
  GNOMEUIINFO_ITEM_NONE("Move to selection", "Moves the focus to the current selection", CB::MW::movetsel),
  GNOMEUIINFO_END
};

static GnomeUIInfo utility_menu[] = {
  GNOMEUIINFO_ITEM_NONE("Convert to degrees", "Converts the angles in the map to degrees", CB::MW::convtod),
  GNOMEUIINFO_ITEM_NONE("Convert to radians", "Converts the angles in the map to radians", CB::MW::convtor),
  GNOMEUIINFO_END
};

static GnomeUIInfo help_menu[] = {
  GNOMEUIINFO_MENU_ABOUT_ITEM(CB::MW::about, NULL),
  GNOMEUIINFO_END
};

static GnomeUIInfo main_menu[] = {
  GNOMEUIINFO_MENU_FILE_TREE(file_menu),
//  GNOMEUIINFO_MENU_EDIT_TREE(edit_menu),
  GNOMEUIINFO_MENU_VIEW_TREE(view_menu),
  GNOMEUIINFO_SUBTREE("Utility", utility_menu),
  GNOMEUIINFO_MENU_HELP_TREE(help_menu),
};

}

#endif
