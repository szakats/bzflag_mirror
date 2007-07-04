#include "../include/windows/EventHandlerCollection.h"

// add an event handler
bool EventHandlerCollection::addEventHandler( const char* _name, BZEventHandler* eventHandler )  {
	string name = string(_name);
	if( this->eventHandlers.count( name ) > 0 )
		return false;
		
	this->eventHandlers[ name ] = eventHandler;
	return true;
}

// remove an event handler
bool EventHandlerCollection::removeEventHandler( const char* _name ) {
	string name = string(_name);
	if( this->eventHandlers.count( name ) <= 0)
		return false;
	
	this->eventHandlers.erase( name );
	return true;
}

// get an event handler
osg::ref_ptr< BZEventHandler > EventHandlerCollection::getEventHandler( const char* name ) {
	return this->eventHandlers[ string(name) ];
}

// set an event handler
osg::ref_ptr< BZEventHandler > EventHandlerCollection::setEventHandler( const char* _name, BZEventHandler* eventHandler ) {
	osg::ref_ptr< BZEventHandler > prevHandler = NULL;
	string name = string(_name);
	
	if( this->eventHandlers.count( name ) > 0 )	
		prevHandler = this->eventHandlers[ name ];
		
	this->eventHandlers[name] = osg::ref_ptr< BZEventHandler >( eventHandler );
	
	return prevHandler;
}

// assign a current event handler
void EventHandlerCollection::makeCurrentHandler( const char* _name )  {
	if(_name == NULL) {
		this->currentEventHandler = NULL;
		return;	
	}
	
	string name = string(_name);
	if( this->eventHandlers.count( name ) > 0 ) {
		this->currentEventHandler = this->eventHandlers[ name ].get();
	}
}

// handle all incoming events
bool EventHandlerCollection::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa ) {
	// if we have a designated event handler, then pass it on
	if( currentEventHandler != NULL ) {
		return currentEventHandler->handle( ea, aa );	
	}
	// otherwise, pass the data on to ALL event handlers
	else if( this->eventHandlers.size() > 0 ) {
		bool handled = false;
		for( map< string, osg::ref_ptr< BZEventHandler > >::iterator i = this->eventHandlers.begin(); i != this->eventHandlers.end() && !handled; i++) {
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
	            	this->pick(viewer,ea);
        	}
       		return false;
       		
        default:
            return false;
    }
}
*/
