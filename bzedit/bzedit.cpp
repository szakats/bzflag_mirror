#include <gnome.h>
#include <cstring>
#include "world.h"
#include "globals.h"

int main(int argc, char **argv) {
  // Initialize GNOME
  GnomeProgram *program;
  program = gnome_program_init("bzedit", "2.1", LIBGNOMEUI_MODULE, argc, argv, NULL);

  string file;
  if (argc >= 2 && *argv[1] != '-') {
    argc--;
    file=argv[1];
    argv = &argv[1];
  }

  // Create our objects
  doc = NULL;
  mainwin = new UI::MainWindow;
  doc = new Document(file);
  mainwin->setsignals();
  mainwin->redraw();

  // Enter our main processing loop
  gtk_main();
}
