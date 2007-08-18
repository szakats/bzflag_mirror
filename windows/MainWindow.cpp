#include "../include/windows/MainWindow.h"

bool MainWindow::initialized = false;

// build up the button panel
void MainWindow::buildButtonPanel() {
	// set up the "add object" button group
	this->objectButtonGroup = new Fl_Group( BUTTON_PANEL_X, BUTTON_PANEL_Y, BUTTON_PANEL_WIDTH, BUTTON_PANEL_HEIGHT, "Add Object");
	this->objectButtonGroup->end();
	this->objectButtonGroup->align( FL_ALIGN_LEFT | FL_ALIGN_TOP );
	
	// create the buttons
	this->addBoxButton = new Fl_ImageButton( BUTTON_PANEL_X, BUTTON_PANEL_Y, 36, 36, "share/UI/box.png");
	this->addPyramidButton = new Fl_ImageButton( BUTTON_PANEL_X + 36, BUTTON_PANEL_Y, 36, 36, "share/UI/pyramid.png");
	this->addTeleporterButton = new Fl_ImageButton( BUTTON_PANEL_X, BUTTON_PANEL_Y + 36, 36, 36, "share/UI/teleporter.png");
	
	// assign them callbacks
	this->addBoxButton->callback( addBoxCallback, this );
	this->addPyramidButton->callback( addPyramidCallback, this );
	this->addTeleporterButton->callback( addTeleporterCallback, this );
	
	// add them to the group
	this->objectButtonGroup->add( addBoxButton );
	this->objectButtonGroup->add( addPyramidButton );
	this->objectButtonGroup->add( addTeleporterButton );
	
	
	// set up the "add base" button group
	this->baseButtonGroup = new Fl_Group( BASE_PANEL_X, BASE_PANEL_Y, BASE_PANEL_WIDTH, BASE_PANEL_HEIGHT, "Add Base");
	this->baseButtonGroup->end();
	this->baseButtonGroup->align( FL_ALIGN_LEFT | FL_ALIGN_TOP );
	
	// create the base buttons
	this->addBlueBaseButton = new Fl_ImageButton( BASE_PANEL_X, BASE_PANEL_Y, 36, 36, "share/UI/bluebase.png");
	this->addGreenBaseButton = new Fl_ImageButton( BASE_PANEL_X + 36, BASE_PANEL_Y, 36, 36, "share/UI/greenbase.png");
	this->addPurpleBaseButton = new Fl_ImageButton( BASE_PANEL_X, BASE_PANEL_Y + 36, 36, 36, "share/UI/purplebase.png");
	this->addRedBaseButton = new Fl_ImageButton( BASE_PANEL_X + 36, BASE_PANEL_Y + 36, 36, 36, "share/UI/redbase.png");
	
	// assign the base buttons callbacks
	this->addBlueBaseButton->callback( addBlueBaseCallback, this );
	this->addGreenBaseButton->callback( addGreenBaseCallback, this );
	this->addPurpleBaseButton->callback( addPurpleBaseCallback, this );
	this->addRedBaseButton->callback( addRedBaseCallback, this );
	
	// add them to the group
	this->baseButtonGroup->add( addBlueBaseButton );
	this->baseButtonGroup->add( addGreenBaseButton );
	this->baseButtonGroup->add( addPurpleBaseButton );
	this->baseButtonGroup->add( addRedBaseButton );
	
	// add the groups
	this->add( objectButtonGroup );
	this->add( baseButtonGroup );
	
	// make the configuration button
	this->configureButton = new Fl_Button( RENDER_WINDOW_X, RENDER_WINDOW_Y + RENDER_WINDOW_HEIGHT, 80, DEFAULT_TEXTSIZE + 6, "Configure" );
	this->configureButton->callback( configureCallback, this );
	this->add( configureButton );
}


// default constructor
MainWindow::MainWindow() :
	Fl_Window(DEFAULT_WIDTH, DEFAULT_HEIGHT, "BZWorkbench") {
	
	this->end();
	
	this->model = new Model();
	this->view = new View(model, this, RENDER_WINDOW_X, RENDER_WINDOW_Y, RENDER_WINDOW_WIDTH, RENDER_WINDOW_HEIGHT);
	view->end();
	this->add(view);
	
	this->configurationMenu = new ConfigurationMenu(this, MENU_X, MENU_Y, MENU_WIDTH, MENU_HEIGHT);
	this->add(configurationMenu);
	
	// this->mainMenu = new MainMenu(this, MENU_X, MENU_Y, MENU_WIDTH, MENU_HEIGHT);
	// this->add(mainMenu);
	
	this->menuBar = new MenuBar( this );
	this->add(menuBar);
	
	initialized = true;
   
   // add the View as an observer to the Model
   model->addObserver( view );
   
   // build the button panel
   this->buildButtonPanel();
}

// construct from a model
MainWindow::MainWindow(Model* model) :
	Fl_Window(DEFAULT_WIDTH, DEFAULT_HEIGHT, "BZWorkbench") {
	
	
	this->end();
	
	this->model = model;
	
	this->view = new View(model, this, RENDER_WINDOW_X, RENDER_WINDOW_Y, RENDER_WINDOW_WIDTH, RENDER_WINDOW_HEIGHT);
	view->end();
	
	this->add(view);
	
	
	this->configurationMenu = new ConfigurationMenu(this, RENDER_WINDOW_X, RENDER_WINDOW_Y, RENDER_WINDOW_WIDTH, RENDER_WINDOW_HEIGHT);
	this->add(configurationMenu);
	
	// this->mainMenu = new MainMenu(this, RENDER_WINDOW_X, RENDER_WINDOW_Y, RENDER_WINDOW_WIDTH, RENDER_WINDOW_HEIGHT);
	// this->add(mainMenu);
	
	this->menuBar = new MenuBar( this );
	this->add(menuBar);
	
	initialized = true;
   
   // add the View as an observer to the Model
   model->addObserver( view );
   
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
					return Fl::handle( event, this->view );
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
	this->configurationMenu->setObject( obj );
	// this->configurationMenu->popup();
}

// throw an error
void MainWindow::error(const char* errorText) {
	Fl_Error* errorMessage = new Fl_Error( errorText );
	errorMessage->show();
}

// launch a MasterConfigurationDialog with the configure button
void MainWindow::configureCallback_real( Fl_Widget* w ) {
	
	vector< bz2object* > selection = this->model->getSelection();
	if( selection.size() > 1 || selection.size() == 0 )
		return;
	
	bz2object* obj = selection[0];
	MasterConfigurationDialog* mcd = new MasterConfigurationDialog( obj );
	mcd->show();
	
	if( this->model->_hasInitializer( obj ) )
		mcd->setAdditionalConfiguration( this->model->_configureObject( obj ) );
}
