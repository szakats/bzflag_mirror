#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Window.H>

#include "../model/Model.h"

#include "../dialogs/MainMenu.h"
#include "../dialogs/WorldOptionsDialog.h"
#include "View.h"
#include "Picker.h"


class MainWindow : public Fl_Window
{
public:

	// default window dimensions
	static const int DEFAULT_WIDTH = 1100;
	static const int DEFAULT_HEIGHT = 700;
	
	// render window position and dimensions
	static const int RENDER_WINDOW_X = 200;
	static const int RENDER_WINDOW_Y = 20;
	static const int RENDER_WINDOW_WIDTH = DEFAULT_WIDTH - RENDER_WINDOW_X - 20;
	static const int RENDER_WINDOW_HEIGHT = DEFAULT_HEIGHT - RENDER_WINDOW_Y - 10;
	
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
	
	// static methods
	static void openDialog(const char* dialog);
	
	// get the view
	View* getView() { return view; }
	
private:
	
	// reference to the main menu
	MainMenu* mainMenu;
	
	// reference to the data model
	Model* model;
	
	// set initialized (checked by static methods)
	static bool initialized;
	
	// reference to the internal OSG view
	View* view;
	
};

#endif /*MAINWINDOW_H_*/
