#include <gnome.h>
#include "ui_main.h"
#include "ui_edit.h"
#include "world.h"
#include "textures.h"

int main(int argc, char **argv) {
  // Initialize GNOME
  gnome_init("bzedit", "2.0", argc, argv);

  // Declare our objects
  UI::MainWindow win;
  World world;

  // Fill the list window
  world.buildList(win.getListWindow().getList());
  win.setWorld(&world);

  // Enter our main processing loop
  gtk_main();
}
