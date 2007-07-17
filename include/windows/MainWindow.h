#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Window.H>

#include "../model/Model.h"
#include "../model/SceneBuilder.h"

#include "../dialogs/MainMenu.h"
#include "../dialogs/ConfigurationMenu.h"
#include "../dialogs/MenuBar.h"
#include "../dialogs/WorldOptionsDialog.h"
#include "../dialogs/MasterConfigurationDialog.h"

#include "../dialogs/Fl_Error.h"

#include <osg/Timer>

class Model;
class View;

/**
 * This is the Controller class in BZWB.  It contains the main window, directs event-handling and UI requests
 * to dialog boxes, and forewards events to the 3D scene.
 */
class MainWindow : public Fl_Window
{
public:

	// default window dimensions
	static const int DEFAULT_WIDTH = 1100;
	static const int DEFAULT_HEIGHT = 700;
	
	// render window position and dimensions
	static const int RENDER_WINDOW_X = 20;
	static const int RENDER_WINDOW_Y = 30;
	static const int RENDER_WINDOW_WIDTH = DEFAULT_WIDTH - RENDER_WINDOW_X - 20;
	static const int RENDER_WINDOW_HEIGHT = DEFAULT_HEIGHT - RENDER_WINDOW_Y - 30;
	
	// menu widget position and dimensions
	static const int MENU_X = RENDER_WINDOW_X;
	static const int MENU_Y = RENDER_WINDOW_Y;
	static const int MENU_WIDTH = RENDER_WINDOW_WIDTH;
	static const int MENU_HEIGHT = RENDER_WINDOW_HEIGHT;
	
	// constructors
	MainWindow();
	MainWindow(Model* model);
	
	// destructor
	virtual ~MainWindow();
	
	// get the view
	View* getView() { return view; }
	
	// get the model
	Model* getModel() { return this->model; }
	
	// configure an object
	void configure( bz2object* obj );
	
	// throw an error message
	void error( const char *text);
	
	// handler
	virtual int handle(int event);
	
	//get/set world name (as an absolute path)
	string getWorldName() { return this->worldName; }
	void setWorldName(const char* name) { this->worldName = name; }
	
private:
	
	// reference to the main menu
	MainMenu* mainMenu;
	
	// reference to the configuration menu
	ConfigurationMenu* configurationMenu;
	
	// reference to the menu bar
	MenuBar* menuBar;
	
	// reference to the data model
	Model* model;
	
	// set initialized (checked by static methods)
	static bool initialized;
	
	// reference to the internal OSG view
	View* view;
	
	// event timer
	osg::Timer timer;
	
	// the name of this world
	string worldName;
	
};

#include "../model/Model.h"
#include "View.h"

#endif /*MAINWINDOW_H_*/
