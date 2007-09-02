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

#include "windows/View.h"

const double View::DEFAULT_ZOOM = 75.0;

// view constructor
View::View(Model* m, MainWindow* mw, int x, int y, int w, int h, const char *label) :
	RenderWindow(x,y,w,h) {
	
	// set OSG viewport
    getCamera()->setViewport(new osg::Viewport(0,0,w,h));
    
    // get the graphics context
    getCamera()->setGraphicsContext(getGraphicsWindow());
    
    // do single-threaded view
   setThreadingModel(osgViewer::Viewer::SingleThreaded);
   
   // set the model
   model = m;
   
   // initialize the root node
   root = new osg::Group();
   
   // configure the stateset of the root node
   osg::StateSet* stateSet = root->getOrCreateStateSet();
   
   // disable OSG's shading by making full white ambient light
   osg::LightModel* lighting = new osg::LightModel();
   lighting->setAmbientIntensity( osg::Vec4( 1, 1, 1, 1 ) );
   
   stateSet->setAttribute( lighting, osg::StateAttribute::OVERRIDE );
   
   stateSet->setTextureMode( 0, GL_TEXTURE_2D, osg::StateAttribute::ON );
   
   // initialize the ground
   ground = new Ground( 400.0f );
   
   // add the ground to the root node
   root->addChild( ground );
	
	// set the key modifiers to false
   modifiers = map< int, bool >();
   modifiers[ FL_SHIFT ] = false;
   modifiers[ FL_CTRL ] = false;
   modifiers[ FL_ALT ] = false;
   modifiers[ FL_META ] = false;
   
	// build the scene from the model
	Model::objRefList objects = model->getObjects();
	if(objects.size() > 0) {
		for(Model::objRefList::iterator i = objects.begin(); i != objects.end(); i++) {
			root->addChild( i->get() );
		}
	}
	
	
   // make a new selection object
   selection = new Selection();
   
   // add the selection
   // NOTE: this has to be the LAST child on the list, because it doesn't have Z-bufferring enabled!
   root->addChild( selection );
   
	// add the root node to the scene
   setSceneData( root );
   
   // give the View a trackball manipulator
   osgGA::TrackballManipulator* cameraManipulator = new osgGA::TrackballManipulator();
   
   // make sure that the center of the trackball doesn't move if we zoom in too close
   cameraManipulator->setMinimumZoomScale( 0.0 );
   
   this->setCameraManipulator(cameraManipulator);
   this->cameraManipulatorRef = cameraManipulator;
   
   // make an event handler collection
   eventHandlers = new EventHandlerCollection( this );
   
   // add the selectHandler
   eventHandlers->addEventHandler( selectHandler::getName().c_str(), new selectHandler( this, cameraManipulator ) );
   
   // add the scene picker event handler
   addEventHandler(eventHandlers);
   
   // assign the parent reference
   mw = mw;
   
   // build the mouse button map
   buildMouseButtonMap();
}

// build the mouse button map
void View::buildMouseButtonMap() {
	mouseButtonMap = map< unsigned int, unsigned int > ();
	
	// default mapppings
	
	mouseButtonMap[ FL_MIDDLE_MOUSE ] = FL_LEFT_MOUSE;		// middle mouse drags in FLTK should translate to left mouse drags in OSG
	
}

// destructor
View::~View() {
	if(eventHandlers)
		delete eventHandlers;
}


// draw method (really simple)
void View::draw(void) {
	frame();
}

// scale the selection based on the distance from the camera to the center to ensure it stays the same size
void View::updateSelection( float newDistance ) {
	this->selection->setScale( osg::Vec3( 0.01 * newDistance, 0.01 * newDistance, 0.01 * newDistance ) );
}

// handle events
int View::handle(int event) {
	// whatever the event was, we need to regenerate the modifier keys
	int shiftState = Fl::event_state();
	modifiers[ FL_SHIFT ] = (( shiftState & FL_SHIFT ) != 0);
	modifiers[ FL_CTRL ] = (( shiftState & FL_CTRL ) != 0);
	modifiers[ FL_META ] = (( shiftState & FL_META ) != 0);
	modifiers[ FL_ALT ] = (( shiftState & FL_ALT ) != 0);
	
	keydown = Fl::event_key();
	buttondown = Fl::event_button();
    
    // set up the 3D cursor by the key
    selection->setStateByKey( keydown );
    
	// forward FLTK events to OSG
	switch(event){
        case FL_PUSH:
        	// handle single mouse clicks
        	if(Fl::event_clicks() == 0) {
        		_gw->getEventQueue()->mouseButtonPress(Fl::event_x(), Fl::event_y(), Fl::event_button() );
        	}
        	// handle double clicks
            else {
            	_gw->getEventQueue()->mouseDoubleButtonPress(Fl::event_x(), Fl::event_y(), Fl::event_button() );
            	Fl::event_is_click(0);
            }
            
            redraw();
            return 1;
          
        case FL_DRAG: {
        	_gw->getEventQueue()->mouseMotion(Fl::event_x(), Fl::event_y());
        	// get the distance from the eyepoint to the center of the trackball
    		float dist = this->cameraManipulatorRef->getDistance();
    		this->updateSelection( dist );
    		// refresh
        	redraw();
			return 1;
        }
        case FL_RELEASE:
            _gw->getEventQueue()->mouseButtonRelease(Fl::event_x(), Fl::event_y(), Fl::event_button() );
        	redraw();    
			return 1;
        case FL_KEYDOWN:
        
        	_gw->getEventQueue()->keyPress((osgGA::GUIEventAdapter::KeySymbol)Fl::event_key());
            redraw();
            return 1;
        case FL_KEYUP:
        	selection->setState( Selection::TRANSLATE);
        	
            _gw->getEventQueue()->keyRelease((osgGA::GUIEventAdapter::KeySymbol)Fl::event_key());
            redraw();
            return 1;
        default:
            // pass other events to the base class
            return RenderWindow::handle(event);
    }
}

// update method (inherited from Observer)
void View::update( Observable* obs, void* data ) {
	
	// refresh the selection
	selection->update( obs, data );
	
	// process data
	if( data != NULL ) {
		// get the message
		ObserverMessage* obs_msg = (ObserverMessage*)(data);
		
		// process the message
		switch( obs_msg->type ) {
			// add an object to the scene
			case ObserverMessage::ADD_OBJECT : {
				bz2object* obj = (bz2object*)(obs_msg->data);
				
				if( getRootNode()->getNumChildren() > 0 )
					getRootNode()->insertChild( 1, obj );	// insert the child directly after the Ground object
				else
					getRootNode()->addChild( obj );
					
					
				break;
			}
			// remove an object from the scene
			case ObserverMessage::REMOVE_OBJECT : {
				bz2object* obj = (bz2object*)(obs_msg->data);
				getRootNode()->removeChild( obj );
				
				break;
			}
			// update the world size
			case ObserverMessage::UPDATE_WORLD : {
				// in this case, the data will contain a pointer to the modified world object
				world* bzworld = (world*)(obs_msg->data);
				
				root->removeChild( ground );
				ground = new Ground( bzworld->getSize(), model->getWaterLevelData()->getHeight() );
				root->insertChild(0, ground);
				
				break;
			}
			// update an object (i.e. it's selection value changed, etc.)
			case ObserverMessage::UPDATE_OBJECT : {
				bz2object* obj = (bz2object*)(obs_msg->data);
				if( obj->isSelected() )
					SceneBuilder::markSelectedAndPreserveStateSet( obj );
				else
					SceneBuilder::markUnselectedAndRestoreStateSet( obj );
					
				break;	
			}
			default:
				break;
		}
	}
	
	// refresh the scene
	redraw();
}

// is a button pressed?
bool View::isPressed( int value ) {
	return modifiers[ value ];
}

bool View::isSelected( bz2object* obj ) { return this->model->isSelected( obj ); }
/**
 * Tell the model to select an object
 */
void View::setSelected( bz2object* object ) {
	
	this->model->_setSelected( object );
}

/**
 * Tell the model to unselect an object
 */
void View::setUnselected( bz2object* object ) {
	
	this->model->_setUnselected( object );
}


/**
 * Tell the model to unselect all selected objects
 */
void View::unselectAll() {
	
	this->model->_unselectAll();
}

