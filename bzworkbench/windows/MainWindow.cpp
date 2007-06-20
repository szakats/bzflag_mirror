#include "../include/windows/MainWindow.h"

bool MainWindow::initialized = false;

// default constructor
MainWindow::MainWindow() :
	Fl_Window(DEFAULT_WIDTH, DEFAULT_HEIGHT, "BZWorkbench") {
	
	this->end();
	
	this->model = new Model();
	
	this->renderWindow = new RenderWindow(RENDER_WINDOW_X, RENDER_WINDOW_Y, RENDER_WINDOW_WIDTH, RENDER_WINDOW_HEIGHT, this->model);
	renderWindow->end();
	
	this->add(renderWindow);
	
	this->mainMenu = new MainMenu(MENU_X, MENU_Y, MENU_WIDTH, MENU_HEIGHT);
	this->add(mainMenu);
	
	initialized = true;
	
}

// construct from a model
MainWindow::MainWindow(Model* model) :
	Fl_Window(DEFAULT_WIDTH, DEFAULT_HEIGHT, "BZWorkbench") {
	
	
	this->end();
	
	this->model = model;
	
	this->renderWindow = new RenderWindow(RENDER_WINDOW_X, RENDER_WINDOW_Y, RENDER_WINDOW_WIDTH, RENDER_WINDOW_HEIGHT, this->model);
	renderWindow->end();
	this->add(renderWindow);
	
	this->mainMenu = new MainMenu(MENU_X, MENU_Y, MENU_WIDTH, MENU_HEIGHT);
	renderWindow->add(mainMenu);
	
	
	initialized = true;
	
}

MainWindow::~MainWindow() {
	if(model)
		delete model;
	
	initialized = false;
	
}

// opens a dialog based on input text
void MainWindow::openDialog(const char* _dialog) {
	if(!initialized)
		return;
		
	string dialog = string(_dialog);
	
	if(dialog.compare("WorldOptionsDialog") == 0) {
		WorldOptionsDialog* wod = new WorldOptionsDialog();
		wod->show();	
	}
}
