#include "MainWindow.h"

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

void MainWindow::openDialog(unsigned int dialog) {
	switch (dialog) {
		case doWorldOptionsDialog:
			new WorldOptionsDialog();
			break;
		
	}
}
