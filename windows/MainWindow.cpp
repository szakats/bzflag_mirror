#include "../include/windows/MainWindow.h"

bool MainWindow::initialized = false;

// default constructor
MainWindow::MainWindow() :
	Fl_Window(DEFAULT_WIDTH, DEFAULT_HEIGHT, "BZWorkbench") {
	
	this->end();
	
	this->model = new Model();
	
	this->view = new View(model, RENDER_WINDOW_X, RENDER_WINDOW_Y, RENDER_WINDOW_WIDTH, RENDER_WINDOW_HEIGHT);
	view->end();
	
	this->add(view);
	
	this->mainMenu = new MainMenu(MENU_X, MENU_Y, MENU_WIDTH, MENU_HEIGHT);
	this->add(mainMenu);
	
	initialized = true;
	
	// give the View a trackball manipulator
   this->view->setCameraManipulator(new osgGA::TrackballManipulator());
   
   // add the stats event handler to the View
   this->view->addEventHandler(new osgViewer::StatsHandler());
   
   // add the scene picker event handler
   this->view->addEventHandler(new Picker(view));
}

// construct from a model
MainWindow::MainWindow(Model* model) :
	Fl_Window(DEFAULT_WIDTH, DEFAULT_HEIGHT, "BZWorkbench") {
	
	
	this->end();
	
	this->model = model;
	
	this->view = new View(model, RENDER_WINDOW_X, RENDER_WINDOW_Y, RENDER_WINDOW_WIDTH, RENDER_WINDOW_HEIGHT);
	view->end();
	this->add(view);
	
	this->mainMenu = new MainMenu(MENU_X, MENU_Y, MENU_WIDTH, MENU_HEIGHT);
	view->add(mainMenu);
	
	
	initialized = true;
	
	// give the View a trackball manipulator
   this->view->setCameraManipulator(new osgGA::TrackballManipulator());
   
   // add the stats event handler to the View
   this->view->addEventHandler(new osgViewer::StatsHandler());
   
   // add the scene picker event handler
   this->view->addEventHandler(new Picker(view));
	
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
	
}
