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

#ifndef VIEW_H_
#define VIEW_H_

#include <osg/Geode>
#include <osg/Group>
#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/StateSet>
#include <osg/Image>
#include <osg/PositionAttitudeTransform>
#include <osgGA/EventQueue>
#include <osg/ShadeModel>
#include <osg/LightModel>

#include "RenderWindow.h"
#include "../model/Model.h"
#include "../model/SceneBuilder.h"

#include "../objects/bz2object.h"
#include "../objects/box.h"
#include "../objects/world.h"

#include "../Observer.h"
#include "../render/Renderable.h"

#include "EventHandlerCollection.h"
#include "eventHandlers/selectHandler.h"

#include "../render/Ground.h"

#include "../render/Selection.h"

#include "../model/ObserverMessage.h"

// forward declare the Picker class
class BZEventHandler;

// forward declare the MainWindow class
class MainWindow;

// an extension of RenderWindow and osgViewer (this will be added to the main window), and Observer
class View : public osgViewer::Viewer, public RenderWindow, public Observer
{
    public:
    	
    	// default zoom
    	static const double DEFAULT_ZOOM;
    	
    	// constructor
        View(Model* m, MainWindow* mw, int x, int y, int w, int h, const char *label=0);
        
        // get the root node
        osg::Group* getRootNode() { return root; }
        
        // FLTK event handler
        virtual int handle(int);
        
        // Observer update() method
        void update( Observable* obs, void* data );
        
        // set an object as selected
        void setSelected( bz2object* object );
        
        // set an object as unselected
        void setUnselected( bz2object* object );
        
        // see if a renderable is contained
       	bool contains( Renderable* node ) { return root->containsNode( node ); }
        
        // select all objects
        // void selectAll();
        
        // unselect all objects
        void unselectAll();
        
        // destructor
        virtual ~View();
        
        // is an object selected?
        bool isSelected( Renderable* node );
        bool isSelected( bz2object* obj );
        
        // is a button down?
        bool isPressed( int value );
        
        // get the current pressed key
        unsigned char getKey() { return keydown; }
        
        // get the current pressed button
        unsigned int getButton() { return buttondown; }
        
        // get the model reference
        const Model* getModelRef() { return model; }
        
        // get the selection reference
        Selection* getSelectionNode() { return selection; }
        
        // get the MainWindow parent, if possible
        MainWindow* requestMainWindow() { return mw; }
        
    protected:
    
    	// draw method
        virtual void draw();
	
		// model reference
		Model* model;
		
		// parent window reference
		MainWindow* mw;
		
		// root node
		osg::Group* root;
		
		// ground node
		Renderable* ground;
		
		// modifier key map.
		// maps FLTK key values to bools
		map< int, bool > modifiers;
		
		// the currently-pressed key
		unsigned char keydown;
		
		// the currently pressed mouse button
		unsigned int buttondown;
		
		// pointer to the trackball matrix manipulator
		osg::ref_ptr< osgGA::TrackballManipulator > cameraManipulatorRef;
	
	private:
		
		// the collection of evnet handlers
		EventHandlerCollection* eventHandlers;
		
		// the current selection
		Selection* selection;
		
		// mouse button map (i.e. so we can adapt user input into input OSG expects)
		map< unsigned int, unsigned int > mouseButtonMap;
		
		// build the mouse button map
		void buildMouseButtonMap();
		
		// update the selection's axes
		void updateSelection( float distance );
};


#endif /*VIEW_H_*/
