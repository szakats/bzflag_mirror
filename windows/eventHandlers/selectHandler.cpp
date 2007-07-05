#include "../../include/windows/eventHandlers/selectHandler.h"

selectHandler::selectHandler() : BZEventHandler() {
	this->lastSelected = NULL;
}

// handle an event
bool selectHandler::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa ) {
	View* viewer;
	
    switch(ea.getEventType())
    {
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

// use the OSG intersection API to pick objects
bool selectHandler::pick(View* viewer, const osgGA::GUIEventAdapter& ea) {
	// quit if the view is invalid
	if( viewer == NULL )
		return false;
		
	// intersections with the scene
    osgUtil::LineSegmentIntersector::Intersections intersections;
	
	// get the intersections from the point in the view where we clicked
    if(viewer->computeIntersections( ea.getX(), ea.getY(), intersections ) ) {
    	// iterate through the intersections
    	for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin(); hitr != intersections.end(); ++hitr) {
            
            if(hitr->nodePath.empty())
            	continue;
            
            for(unsigned int i = 0; i < hitr->nodePath.size(); i++) {
            	// only look for Renderables
            	Renderable* obj = dynamic_cast< Renderable* > ( hitr->nodePath[i] );
            	if(obj != NULL && obj->getName().length() > 0) {
            		
            		if(!viewer->isPressed( FL_SHIFT )) {
            			viewer->unselectAll();
            		}
            		
					if(!viewer->isSelected( obj )) {
						viewer->setSelected( obj );	
					}
					else {
						viewer->setUnselected( obj );
					}
            		
            		// save the last selected object
            		this->lastSelected = obj;
            		
            		return true;
            	}
            }
            
        }
    }
    return false;
}
