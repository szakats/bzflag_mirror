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

#include "windows/MainWindow.h"

bool MainWindow::initialized = false;

// build up the button panel
void MainWindow::buildButtonPanel() {
	// set up the "add object" button group
	objectButtonGroup = new Fl_Group( BUTTON_PANEL_X, BUTTON_PANEL_Y, BUTTON_PANEL_WIDTH, BUTTON_PANEL_HEIGHT, "Add Object");
	objectButtonGroup->end();
	objectButtonGroup->align( FL_ALIGN_LEFT | FL_ALIGN_TOP );
	
	// create the buttons
	addBoxButton = new Fl_ImageButton( BUTTON_PANEL_X, BUTTON_PANEL_Y, 36, 36, "share/UI/box.png");
	addPyramidButton = new Fl_ImageButton( BUTTON_PANEL_X + 36, BUTTON_PANEL_Y, 36, 36, "share/UI/pyramid.png");
	addTeleporterButton = new Fl_ImageButton( BUTTON_PANEL_X, BUTTON_PANEL_Y + 36, 36, 36, "share/UI/teleporter.png");
	
	// assign them callbacks
	addBoxButton->callback( addBoxCallback, this );
	addPyramidButton->callback( addPyramidCallback, this );
	addTeleporterButton->callback( addTeleporterCallback, this );
	
	// add them to the group
	objectButtonGroup->add( addBoxButton );
	objectButtonGroup->add( addPyramidButton );
	objectButtonGroup->add( addTeleporterButton );
	
	
	// set up the "add base" button group
	baseButtonGroup = new Fl_Group( BASE_PANEL_X, BASE_PANEL_Y, BASE_PANEL_WIDTH, BASE_PANEL_HEIGHT, "Add Base");
	baseButtonGroup->end();
	baseButtonGroup->align( FL_ALIGN_LEFT | FL_ALIGN_TOP );
	
	// create the base buttons
	addBlueBaseButton = new Fl_ImageButton( BASE_PANEL_X, BASE_PANEL_Y, 36, 36, "share/UI/bluebase.png");
	addGreenBaseButton = new Fl_ImageButton( BASE_PANEL_X + 36, BASE_PANEL_Y, 36, 36, "share/UI/greenbase.png");
	addPurpleBaseButton = new Fl_ImageButton( BASE_PANEL_X, BASE_PANEL_Y + 36, 36, 36, "share/UI/purplebase.png");
	addRedBaseButton = new Fl_ImageButton( BASE_PANEL_X + 36, BASE_PANEL_Y + 36, 36, 36, "share/UI/redbase.png");
	
	// assign the base buttons callbacks
	addBlueBaseButton->callback( addBlueBaseCallback, this );
	addGreenBaseButton->callback( addGreenBaseCallback, this );
	addPurpleBaseButton->callback( addPurpleBaseCallback, this );
	addRedBaseButton->callback( addRedBaseCallback, this );
	
	// add them to the group
	baseButtonGroup->add( addBlueBaseButton );
	baseButtonGroup->add( addGreenBaseButton );
	baseButtonGroup->add( addPurpleBaseButton );
	baseButtonGroup->add( addRedBaseButton );
	
	// add the groups
	add( objectButtonGroup );
	add( baseButtonGroup );
	
	// make the configuration button
	configureButton = new Fl_Button( RENDER_WINDOW_X, RENDER_WINDOW_Y + RENDER_WINDOW_HEIGHT, 80, DEFAULT_TEXTSIZE + 6, "Configure" );
	configureButton->callback( configureCallback, this );
	add( configureButton );
}

// default constructor
MainWindow::MainWindow() :
	Fl_Window(DEFAULT_WIDTH, DEFAULT_HEIGHT, "BZWorkbench") {
	
	this->end();
	
	this->model = new Model();
	this->view = new View(model, this, RENDER_WINDOW_X, RENDER_WINDOW_Y, RENDER_WINDOW_WIDTH, RENDER_WINDOW_HEIGHT);
	this->view->end();
	this->add(view);
	
	this->configurationMenu = new ConfigurationMenu(this, MENU_X, MENU_Y, MENU_WIDTH, MENU_HEIGHT);
	this->add(configurationMenu);
	
	// mainMenu = new MainMenu(this, MENU_X, MENU_Y, MENU_WIDTH, MENU_HEIGHT);
	// add(mainMenu);
	
	this->menuBar = new MenuBar( this );
	this->add(menuBar);
	
	initialized = true;
   
   // add the View as an observer to the Model
   this->model->addObserver( view );
   
   // build the button panel
   this->buildButtonPanel();
}

// construct from a model
MainWindow::MainWindow(Model* model) :
	Fl_Window(DEFAULT_WIDTH, DEFAULT_HEIGHT, "BZWorkbench") {
	
	
	this->end();
	
	this->model = model;
	
	this->view = new View(this->model, this, RENDER_WINDOW_X, RENDER_WINDOW_Y, RENDER_WINDOW_WIDTH, RENDER_WINDOW_HEIGHT);
	this->view->end();
	
	this->add(view);
	
	
	this->configurationMenu = new ConfigurationMenu(this, RENDER_WINDOW_X, RENDER_WINDOW_Y, RENDER_WINDOW_WIDTH, RENDER_WINDOW_HEIGHT);
	this->add(configurationMenu);
	
	// mainMenu = new MainMenu(this, RENDER_WINDOW_X, RENDER_WINDOW_Y, RENDER_WINDOW_WIDTH, RENDER_WINDOW_HEIGHT);
	// add(mainMenu);
	
	this->menuBar = new MenuBar( this );
	this->add(menuBar);
	
	initialized = true;
   
   // add the View as an observer to the Model
   this->model->addObserver( view );
   
   // build the button panel
   this->buildButtonPanel();
	
}

MainWindow::~MainWindow() {
	if(model)
		delete model;
	
	initialized = false;
	
}

// handle
int MainWindow::handle(int event) {
	
	switch( event ) {
		// catch right-click pushes--the user should double-click to access the menu
		case FL_PUSH:
			if( Fl::event_button() == FL_RIGHT_MOUSE) {
				if( Fl::event_clicks() > 0 ) {
					return Fl_Window::handle( event );
				}
				else {
					return Fl::handle( event, view );
				}
			}
			return Fl_Window::handle( event );
			
		case FL_CLOSE:	// catch window close event
			// make sure to de-allocate nodes
			SceneBuilder::shutdown();
			return 1;
		default:
				
			return Fl_Window::handle( event );
	}
}

// configure an object
void MainWindow::configure( bz2object* obj ) {
	configurationMenu->setObject( obj );
	// configurationMenu->popup();
}

// throw an error
void MainWindow::error(const char* errorText) {
	Fl_Error* errorMessage = new Fl_Error( errorText );
	errorMessage->show();
}

// launch a MasterConfigurationDialog with the configure button
void MainWindow::configureCallback_real( Fl_Widget* w ) {
	
	Model::objRefList selection = model->getSelection();
	if( selection.size() > 1 || selection.size() == 0 )
		return;
	
	bz2object* obj = selection[0].get();
	MasterConfigurationDialog* mcd = new MasterConfigurationDialog( obj );
	mcd->show();
	
	if( model->_hasInitializer( obj ) )
		mcd->setAdditionalConfiguration( model->_configureObject( obj ) );
}
