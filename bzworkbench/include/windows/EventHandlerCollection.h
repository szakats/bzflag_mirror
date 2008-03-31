/* BZWorkbench
 * Copyright (c) 1993 - 2008 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef PICKER_H_
#define PICKER_H_

#include <osgUtil/Optimizer>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

#include <osg/Material>
#include <osg/Geode>
#include <osg/BlendFunc>
#include <osg/Depth>
#include <osg/Projection>
#include <osg/MatrixTransform>
#include <osg/Camera>
#include <osg/Group>
#include <osg/PositionAttitudeTransform>
#include <osg/io_utils>
#include <osg/Node>

#include <vector>
#include <set>

#include "../objects/bz2object.h"

class View;
class BZEventHandler;

using namespace std;

/**
 * This class contains osgGA::GUIEventHandler instances for management, 
 * because OSG's Viewer class can't remove them.  It collects events from the
 * OSG Viewer class and passes them on to its collection.
 * 
 * NOTE: in the default construction, events are forwarded to ALL event handlers.  However,
 * a call to makeCurrentHandler will make it so events are forwarded to ONLY that handler.
 * Calling makeCurrentHandler(NULL) will undo that.
 */
class EventHandlerCollection : public osgGA::GUIEventHandler {
public: 

    EventHandlerCollection( const View* _view ) { this->view = _view; currentEventHandler = NULL; }
    ~EventHandlerCollection() {}
    
    // handle an event
    bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
    
    // add an event handler
    bool addEventHandler( const char* _name, BZEventHandler* eventHandler );
    
    // remove an event handler
    bool removeEventHandler( const char* _name );
	
	// get an event handler
	osg::ref_ptr< BZEventHandler > getEventHandler( const char* name );
	
	// set an event handler
	osg::ref_ptr< BZEventHandler > setEventHandler( const char* name, BZEventHandler* handler );
	
	// make an event handler current
	// pass NULL to allow events to pass to all handlers
	void makeCurrentHandler( const char* _name );
   	
private:
	// the view we're attached to
	const View* view;
	
	// the current event handler
	BZEventHandler* currentEventHandler;
	
	// map of event handlers
	map< string, osg::ref_ptr<BZEventHandler> > eventHandlers;
};

#include "View.h"

#endif /*PICKER_H_*/
