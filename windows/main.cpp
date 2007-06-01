#include <stdio.h>
#include <stdlib.h>

#include "include/defines.h"
#include "include/Fl_Dialog.h"
#include "include/QuickLabel.h"
#include "include/WorldOptionsDialog.h"
#include "include/MainMenu.h"

#include "include/Model.h"

#include "include/MainWindow.h"

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
