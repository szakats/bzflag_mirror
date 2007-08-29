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

#include "windows/EventHandlerCollection.h"

// add an event handler
bool EventHandlerCollection::addEventHandler( const char* _name, BZEventHandler* eventHandler )  {
	string name = string(_name);
	if( eventHandlers.count( name ) > 0 )
		return false;
		
	eventHandlers[ name ] = eventHandler;
	return true;
}

// remove an event handler
bool EventHandlerCollection::removeEventHandler( const char* _name ) {
	string name = string(_name);
	if( eventHandlers.count( name ) <= 0)
		return false;
	
	eventHandlers.erase( name );
	return true;
}

// get an event handler
osg::ref_ptr< BZEventHandler > EventHandlerCollection::getEventHandler( const char* name ) {
	return eventHandlers[ string(name) ];
}

// set an event handler
osg::ref_ptr< BZEventHandler > EventHandlerCollection::setEventHandler( const char* _name, BZEventHandler* eventHandler ) {
	osg::ref_ptr< BZEventHandler > prevHandler = NULL;
	string name = string(_name);
	
	if( eventHandlers.count( name ) > 0 )	
		prevHandler = eventHandlers[ name ];
		
	eventHandlers[name] = osg::ref_ptr< BZEventHandler >( eventHandler );
	
	return prevHandler;
}

// assign a current event handler
void EventHandlerCollection::makeCurrentHandler( const char* _name )  {
	if(_name == NULL) {
		currentEventHandler = NULL;
		return;	
	}
	
	string name = string(_name);
	if( eventHandlers.count( name ) > 0 ) {
		currentEventHandler = eventHandlers[ name ].get();
	}
}

// handle all incoming events
bool EventHandlerCollection::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa ) {
	// if we have a designated event handler, then pass it on
	if( currentEventHandler != NULL ) {
		return currentEventHandler->handle( ea, aa );	
	}
	// otherwise, pass the data on to ALL event handlers
	else if( eventHandlers.size() > 0 ) {
		bool handled = false;
		for( map< string, osg::ref_ptr< BZEventHandler > >::iterator i = eventHandlers.begin(); i != eventHandlers.end() && !handled; i++) {
			handled = i->second->handle( ea, aa );
		}
		return handled;
	}
	else
		return false;
}
	
/*
// the event handler (takes events from OSG GuiAdapters)
// just pick them; don't eliminate the event from the queue
bool Picker::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	View* viewer;
	
    switch(ea.getEventType())
    {
    	// catch single click events
    	case osgGA::GUIEventAdapter::PUSH :
    		return false;
    		
    	// catch double click events and do a pick
        case osgGA::GUIEventAdapter::DOUBLECLICK :
        	if( ea.getButton() == FL_LEFT_MOUSE ) {
	            viewer = dynamic_cast<View*>(&aa);
	            if (viewer)
	            	pick(viewer,ea);
        	}
       		return false;
       		
        default:
            return false;
    }
}
*/
