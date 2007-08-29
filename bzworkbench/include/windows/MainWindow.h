/* BZWorkbench
 * Copyright (c) 1993 - 2007 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>

#include "model/Model.h"
#include "model/SceneBuilder.h"

#include "dialogs/MainMenu.h"
#include "dialogs/ConfigurationMenu.h"
#include "dialogs/MenuBar.h"
#include "dialogs/WorldOptionsDialog.h"
#include "dialogs/MasterConfigurationDialog.h"

#include "dialogs/ConeConfigurationDialog.h"

#include "dialogs/Fl_Error.h"

#include "widgets/Fl_ImageButton.h"

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
	static const int RENDER_WINDOW_X = 110;
	static const int RENDER_WINDOW_Y = 30;
	static const int RENDER_WINDOW_WIDTH = DEFAULT_WIDTH - RENDER_WINDOW_X - 20;
	static const int RENDER_WINDOW_HEIGHT = DEFAULT_HEIGHT - RENDER_WINDOW_Y - 30;
	
	// button panel position and dimensions
	static const int BUTTON_PANEL_X = 5;
	static const int BUTTON_PANEL_Y = 70;
	static const int BUTTON_PANEL_WIDTH = RENDER_WINDOW_X - 10;
	static const int BUTTON_PANEL_HEIGHT = 192;
	
	// base panel position and dimensions
	static const int BASE_PANEL_X = BUTTON_PANEL_X;
	static const int BASE_PANEL_Y = BUTTON_PANEL_Y + BUTTON_PANEL_HEIGHT + 20;
	static const int BASE_PANEL_WIDTH = BUTTON_PANEL_WIDTH;
	static const int BASE_PANEL_HEIGHT = 80;
	
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
	Model* getModel() { return model; }
	
	// configure an object
	void configure( bz2object* obj );
	
	// throw an error message
	void error( const char *text);
	
	// handler
	virtual int handle(int event);
	
	//get/set world name (as an absolute path)
	string getWorldName() { return worldName; }
	void setWorldName(const char* name) { worldName = name; }
	
	// static callbacks for buttons
	static void addBoxCallback(Fl_Widget* w, void* data) {
		MainWindow* mw = (MainWindow*)(data);
		mw->menuBar->addBoxCallback_real( w );
	}
	
	static void addPyramidCallback(Fl_Widget* w, void* data) {
		MainWindow* mw = (MainWindow*)(data);
		mw->menuBar->addPyramidCallback_real( w );
	}
	
	static void addTeleporterCallback(Fl_Widget* w, void* data) {
		MainWindow* mw = (MainWindow*)(data);
		mw->menuBar->addTeleporterCallback_real( w );
	}
	
	static void addBlueBaseCallback(Fl_Widget* w, void* data) {
		MainWindow* mw = (MainWindow*)(data);
		mw->menuBar->addBlueBaseCallback_real( w );
	}
	
	static void addGreenBaseCallback(Fl_Widget* w, void* data) {
		MainWindow* mw = (MainWindow*)(data);
		mw->menuBar->addGreenBaseCallback_real( w );
	}
	
	static void addPurpleBaseCallback(Fl_Widget* w, void* data) {
		MainWindow* mw = (MainWindow*)(data);
		mw->menuBar->addPurpleBaseCallback_real( w );
	}
	
	static void addRedBaseCallback(Fl_Widget* w, void* data) {
		MainWindow* mw = (MainWindow*)(data);
		mw->menuBar->addRedBaseCallback_real( w );
	}
	
	static void configureCallback(Fl_Widget* w, void* data) {
		MainWindow* mw = (MainWindow*)(data);
		mw->configureCallback_real( w );
	}
	
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
	
	// button console
	Fl_Group* buttonConsole;
	
	// object button group
	Fl_Group* objectButtonGroup;
	
	// buttons within the objectButtonGroup that add objects
	Fl_ImageButton* addBoxButton;
	Fl_ImageButton* addPyramidButton;
	Fl_ImageButton* addTeleporterButton;
	
	// base button group
	Fl_Group* baseButtonGroup;
	
	// buttons that add bases
	Fl_ImageButton* addBlueBaseButton;
	Fl_ImageButton* addGreenBaseButton;
	Fl_ImageButton* addPurpleBaseButton;
	Fl_ImageButton* addRedBaseButton;
	
	void configureCallback_real( Fl_Widget* w );
	
	// build the button panel
	void buildButtonPanel();
	
	// button to launch a MasterConfigurationDialog
	Fl_Button* configureButton;
};

#include "model/Model.h"
#include "View.h"

#endif /*MAINWINDOW_H_*/
