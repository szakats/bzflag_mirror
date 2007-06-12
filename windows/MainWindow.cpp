#include "../include/windows/MainWindow.h"

MainWindow::MainWindow() :
	Fl_Window(DEFAULT_WIDTH, DEFAULT_HEIGHT, "BZWorkbench") {
	
	this->end();
	
	this->mainMenu = new MainMenu(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);
	this->add(mainMenu);
	
	
	this->model = new Model();
}

MainWindow::MainWindow(Model* model) :
	Fl_Window(DEFAULT_WIDTH, DEFAULT_HEIGHT, "BZWorkbench") {
	
	this->end();
	
	this->mainMenu = new MainMenu(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);
	this->add(mainMenu);
	
	this->model = model;
}

MainWindow::~MainWindow() {
	if(model)
		delete model;
}

void MainWindow::openDialog(const char* _dialog) {
	string dialog = string(_dialog);
	
	if(dialog.compare("WorldOptionsDialog") == 0) {
		WorldOptionsDialog* wod = new WorldOptionsDialog();
		wod->show();	
	}
}
