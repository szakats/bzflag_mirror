#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "dialogs/Fl_Dialog.h"
#include "dialogs/QuickLabel.h"
#include "dialogs/WorldOptionsDialog/WorldOptionsDialog.h"
#include "dialogs/MainMenu/MainMenu.h"

#include "model/Model.h"

#include "windows/MainWindow.h"

void OK_handler(Fl_Widget* w, void* data) {
	printf("clicked OK\n");
}

void CANCEL_handler(Fl_Widget* w, void* data) {
	printf("clicked CANCEL\n");
}

int main(int argc, char** argv) {
	
	MainWindow* win = new MainWindow(new Model());
	win->show(argc, argv);
	
	return Fl::run();	
}
