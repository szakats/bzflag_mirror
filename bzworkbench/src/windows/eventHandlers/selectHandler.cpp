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

#include "windows/eventHandlers/selectHandler.h"

selectHandler::selectHandler( View* _view, osgGA::MatrixManipulator* manipulator ) : BZEventHandler( _view ) {
	lastSelected = NULL;
	lastSelectedData = NULL;
	dx = dy = prev_x = prev_y = 0.0;
	cameraManipulator = manipulator;
}

// handle an event
bool selectHandler::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa ) {
	View* viewer;
	
    switch(ea.getEventType())
    {
    	
    	// catch drag events
    	case osgGA::GUIEventAdapter::DRAG :
    		
    		viewer = dynamic_cast<View*>(&aa);
    		
    		if( viewer != NULL && lastSelected != NULL && lastSelected->getName() == Selection_NODE_NAME ) {
    			// if the last event was a DRAG event, we need to update the dx and dy
    			if( prevEvent == osgGA::GUIEventAdapter::DRAG ) {
	    			dx = ea.getXnormalized() - prev_x;
	    			dy = ea.getYnormalized() - prev_y;
    			}
    			// otherwise, they should be zero
    			else {
    				dx = 0;
    				dy = 0;	
    			}
    			// set the prev_x and prev_y values so we can re-compute dx and dy on the next event
	    		prev_x = ea.getXnormalized();
    			prev_y = ea.getYnormalized();
    			
    			// log this event
    			prevEvent = osgGA::GUIEventAdapter::DRAG;
    			
    			// handle a selector, based on a pressed key
    			switch( viewer->getKey() ) {
    				// 'r' is for "rotate" (i.e. "spin" in BZW)
    				case BZ_ROTATE_KEY:
    					return rotateSelector( view, ea );
    				
    				// 's' is for "scale"
    				case BZ_SCALE_KEY:
    					return scaleSelector( view, ea );
    				
    				// 'l' is for "lean" (i.e. "shear" in BZW)
    				case BZ_SHEAR_KEY:
    					return shearSelector( view, ea );
    				
    				// 't' is for "translate" (i.e. "shift" in BZW)
    				case BZ_SHIFT_KEY:
    					return shiftSelector( view, ea );
    					
    				default:
    					return dragSelector( view, ea );
    			}
    			
    		}
    		return false;
    		
    	// catch single-click events (see if we picked the selector or an object)
       	case osgGA::GUIEventAdapter::PUSH : {
       		int button = ea.getButton();
       		
       		if( button == FL_LEFT_MOUSE ) {
       			viewer = dynamic_cast<View*>(&aa);
       			if( viewer ) {
       				prevEvent = osgGA::GUIEventAdapter::PUSH;
       				return pickSelector( viewer, ea );
       			}	
       		}
       		else if( button == FL_RIGHT_MOUSE ) {
        		viewer = dynamic_cast<View*>(&aa);
        		if(viewer) {
        			prevEvent = osgGA::GUIEventAdapter::PUSH;
        			return configureObject(viewer, ea);
        			
        		}	
        	}
       		return false;
       	}
    	// catch double click events and do a pick
        case osgGA::GUIEventAdapter::DOUBLECLICK :
        	if( ea.getButton() == FL_LEFT_MOUSE ) {
	            viewer = dynamic_cast<View*>(&aa);
	            if (viewer) {
	            	prevEvent = osgGA::GUIEventAdapter::DOUBLECLICK;
       		
	            	return pickObject(viewer,ea);
	            	
	            }
        	}
        	
       		return false;
       		
        default:
        	// assume we handled everything
            return true;
    }
}

// use the OSG intersection API to pick objects
bool selectHandler::pickObject(View* viewer, const osgGA::GUIEventAdapter& ea) {
		
	// intersections with the scene
    osgUtil::LineSegmentIntersector::Intersections intersections;
	
	// get the intersections from the point in the view where we clicked
    if(viewer->computeIntersections( ea.getX(), ea.getY(), intersections ) ) {
    	// iterate through the intersections
    	for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin(); hitr != intersections.end(); ++hitr) {
            
            if(hitr->nodePath.empty())
            	continue;
            
            for(unsigned int i = 0; i < hitr->nodePath.size(); i++) {
            	// only look for bz2objects
            	bz2object* obj = dynamic_cast< bz2object* > ( hitr->nodePath[i] );
            	if(obj != NULL) {
            	   	
            		if(!viewer->isPressed( FL_SHIFT )) {
            			viewer->unselectAll();
            		}
            		
					if(!viewer->isSelected( obj )) {
						viewer->setSelected( obj );
						// viewer->requestMainWindow()->configure( obj ); 
					}
					else {
						viewer->setUnselected( obj );
					}
					
            		// save the last selected object
            		lastSelected = obj;
            		
            		return true;
            	}
            }
            
        }
    }
    
    lastSelected = NULL;
    lastSelectedData = NULL;
    return false;
}

// use the OSG intersection API to pick objects
bool selectHandler::pickSelector(View* viewer, const osgGA::GUIEventAdapter& ea) {
	
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
            	Selection* obj = dynamic_cast< Selection* > ( hitr->nodePath[i] );
            	if(obj != NULL) {
            		// determine which part of the selector got picked
        			osg::Node* pickedNode = Selection::getPickedNode( obj, hitr->nodePath, i+1 );
        			
            		// save the last selected object
            		lastSelected = obj;
            		
            		// save the node that was picked
            		lastSelectedData = pickedNode;
            		
            		return true;
            	}
            }
        }
    }
    
    lastSelected = NULL;
    lastSelectedData = NULL;
    return false;
}

// see if we picked an object and if so, configure it
// TODO: get this to work properly
bool selectHandler::configureObject(View* viewer, const osgGA::GUIEventAdapter& ea) {
		
	// intersections with the scene
    osgUtil::LineSegmentIntersector::Intersections intersections;
	
	// get the intersections from the point in the view where we clicked
    if(viewer->computeIntersections( ea.getX(), ea.getY(), intersections ) ) {
    	// iterate through the intersections
    	for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin(); hitr != intersections.end(); ++hitr) {
            
            if(hitr->nodePath.empty())
            	continue;
            
            for(unsigned int i = 0; i < hitr->nodePath.size(); i++) {
            	// only look for bz2objects
            	bz2object* obj = dynamic_cast< bz2object* > ( hitr->nodePath[i] );
            	if(obj != NULL/* && obj->isSelected()*/) {
            	   	// tell the MainWindow to open up a configuration menu
            	   	MainWindow* mw = viewer->requestMainWindow();
            	   	
            	   	if(mw) {
            	   		mw->configure( obj );
            	   		lastSelected = obj;
            	   		return true;	
            	   	}
            	   	
            	   	return false;
            	}
            }
            
        }
    }
    
    lastSelected = NULL;
    lastSelectedData = NULL;
    return false;
}

// handle translation events
bool selectHandler::dragSelector( View* viewer, const osgGA::GUIEventAdapter& ea ) {
	osg::Node* node = (osg::Node*)lastSelectedData;
	
	// get the position of the last selected object (which should be the axes)
	osg::Vec3 position = lastSelected->getPosition();
	
	// transform the 2D mouse movement into a 3D vector by transforming it into camera space
	// get the vectors (but keep in mind that the window uses the XY-plane, but "up" in the 3D scene is along Z)
	osg::Vec3 sideVector = cameraManipulator->getSideVector( cameraManipulator->getMatrix() );
	osg::Vec3 upVector = cameraManipulator->getFrontVector( cameraManipulator->getMatrix() );
	
	// apply the transformation to each axis
	sideVector *= ( dx );
	upVector *= ( dy );
	
	// combine them into the transformation vector
	osg::Vec3 transformVector = sideVector + upVector;
	
	// scale the transformVector by the distance from the eyepoint to the axis
	// first, get the eye position of the camera
	osg::Vec3 eye, look, dist;
	viewer->getCamera()->getViewMatrixAsLookAt( eye, look, dist );
	
	// compute the vector between the camera and the axis
	osg::Vec3 dPosition = position - eye;
	
	// compute the dist
	double distance = dPosition.length() / 2;
	
	// finally, scale the transformVector
	transformVector *= distance;
	
	if(node->getName() == Selection_X_AXIS_NODE_NAME) {
		// translate along x
		position.set( position.x() +  transformVector.x(), position.y(), position.z() );
	}
	else if(node->getName() == Selection_Y_AXIS_NODE_NAME) {
		// translate along y
		position.set( position.x(), position.y() + transformVector.y(), position.z() );	
	}
	else if(node->getName() == Selection_Z_AXIS_NODE_NAME) {
		// translate along z
		position.set( position.x(), position.y(), position.z() + transformVector.z() );	
	}
	
	// set the position
	Selection* selection = dynamic_cast< Selection* >( lastSelected );
	
	// update all objects in the selection
	if(selection) {
		
		// get the model reference from the View
		const Model* model = view->getModelRef();
		
		// get the selected objects
		Model::objRefList selected = model->getSelection();
		
		// transform them
		if(selected.size() > 0) {
			osg::Vec3 _dPosition = position - selection->getPosition();
			osg::Vec3 tmp;
			for(Model::objRefList::iterator i = selected.begin(); i != selected.end(); i++) {
				tmp = (*i)->getPos() + _dPosition;
				
				// tell the object it got updated (i.e. so it can handle any changes specific to itself)
				UpdateMessage msg = UpdateMessage( UpdateMessage::SET_POSITION, &tmp );
				(*i)->update( msg );
			}
			
			// finally, transform the selector itself
			selection->rebuildAxes( selected );
		}
	}
		
	return true;
}

// handle rotate events
bool selectHandler::rotateSelector( View* viewer, const osgGA::GUIEventAdapter& ea ) {
	// get the clicked axis
	osg::Node* node = (osg::Node*)lastSelectedData;
	
	// get the position of the selector
	osg::Vec3 position = lastSelected->getPosition();
	
	// get the angular orientation
	double a_x = 0.0, a_y = 0.0, a_z = 0.0;
	
	// transform the 2D mouse movement into a 3D vector by transforming it into camera space
	// get the vectors (but keep in mind that the window uses the XY-plane, but "up" in the 3D scene is along Z)
	osg::Vec3 sideVector = cameraManipulator->getSideVector( cameraManipulator->getMatrix() );
	osg::Vec3 upVector = cameraManipulator->getFrontVector( cameraManipulator->getMatrix() );
	
	// apply the transformation to each axis (only look at movement perpendicular to the axes here, since we're rotating)
	sideVector *= ( dx );
	upVector *= ( dy );
	
	// combine them into the transformation vector
	osg::Vec3 transformVector = sideVector + upVector;
	transformVector.normalize();
	
	if(node->getName() == Selection_X_AXIS_NODE_NAME) {
		// rotate x
		a_x += transformVector.x();
	}
	else if(node->getName() == Selection_Y_AXIS_NODE_NAME) {
		// rotate y
		a_y += transformVector.y();	
	}
	else if(node->getName() == Selection_Z_AXIS_NODE_NAME) {
		// rotate z
		a_z += transformVector.z();
	}
	
	// set the position
	Selection* selection = dynamic_cast< Selection* >( lastSelected );
	
	// update all objects in the selection
	if(selection) {
		// get the model reference from the View
		const Model* model = view->getModelRef();
		
		// get the selected objects
		Model::objRefList selected = model->getSelection();
		
		// transform them
		if(selected.size() > 0) {
			osg::Vec3 dr = osg::Vec3( a_x, a_y, a_z );
			for(Model::objRefList::iterator i = selected.begin(); i != selected.end(); i++) {
				// tell the object it got updated (i.e. so it can handle any changes specific to itself)
				UpdateMessage msg = UpdateMessage( UpdateMessage::SET_ROTATION_FACTOR, &dr );
				
				(*i)->update( msg );
			}
			
			// finally, transform the selector itself
			selection->rebuildAxes( selected );
		}
	}
	
	return true;	
}

// handle scale events
bool selectHandler::scaleSelector( View* viewer, const osgGA::GUIEventAdapter& ea ) {
	osg::Node* node = (osg::Node*)lastSelectedData;
	
	// get the position of the last selected object (which should be the axes)
	osg::Vec3 scale = osg::Vec3( 0, 0, 0 );
	
	// transform the 2D mouse movement into a 3D vector by transforming it into camera space
	// get the vectors (but keep in mind that the window uses the XY-plane, but "up" in the 3D scene is along Z)
	osg::Vec3 sideVector = cameraManipulator->getSideVector( cameraManipulator->getMatrix() );
	osg::Vec3 upVector = cameraManipulator->getFrontVector( cameraManipulator->getMatrix() );
	
	// apply the transformation to each axis
	sideVector *= ( dx );
	upVector *= ( dy );
	
	// combine them into the transformation vector
	osg::Vec3 transformVector = sideVector + upVector;
	transformVector.normalize();
	
	
	if(node->getName() == Selection_X_AXIS_NODE_NAME) {
		// scale along x
		scale.set( scale.x() +  transformVector.x(), scale.y(), scale.z() );
	}
	else if(node->getName() == Selection_Y_AXIS_NODE_NAME) {
		// scale along y
		scale.set( scale.x(), scale.y() + transformVector.y(), scale.z() );	
	}
	else if(node->getName() == Selection_Z_AXIS_NODE_NAME) {
		// scale along z
		scale.set( scale.x(), scale.y(), scale.z() + transformVector.z() );	
	}
	else if(node->getName() == Selection_CENTER_NODE_NAME) {
		// scale along all three axes if we select the center
		scale.set( scale.x() + transformVector.x(), scale.y() + transformVector.y(), scale.z() + transformVector.z());	
	}
	
	// set the position
	Selection* selection = dynamic_cast< Selection* >( lastSelected );
	// update all objects in the selection
	
	if(selection) {
		Model::objRefList selected = view->getModelRef()->getSelection();
		if( selected.size() > 0 ) {
			osg::Vec3 tscale;
			for(Model::objRefList::iterator i = selected.begin(); i != selected.end(); i++) {
				tscale = (*i)->getSize() + scale;
				// no negative scaling!
				if( tscale.x() < 0 )
					scale.set( 0, scale.y(), scale.z() );
				if( tscale.y() < 0 )
					scale.set( scale.x(), 0, scale.z() );
				if( tscale.z() < 0 )
					scale.set( scale.x(), scale.y(), 0 );
				
				// tell the object it got updated (i.e. so it can handle any changes specific to itself)
				// this needs to be done for BZW 1.x objects so their textures scale appropriately
				UpdateMessage msg = UpdateMessage( UpdateMessage::SET_SCALE_FACTOR, &scale );
				(*i)->update( msg );
			}	
		}
	}
	
	return true;	
}

// shear the selector (i.e. if the appropriate key is pressed)
bool selectHandler::shearSelector( View* viewer, const osgGA::GUIEventAdapter& ea) {
	osg::Node* node = (osg::Node*)lastSelectedData;
	
	// shear movements
	double s_x = 0.0, s_y = 0.0, s_z = 0.0;
	
	// transform the 2D mouse movement into a 3D vector by transforming it into camera space
	// get the vectors (but keep in mind that the window uses the XY-plane, but "up" in the 3D scene is along Z)
	osg::Vec3 sideVector = cameraManipulator->getSideVector( cameraManipulator->getMatrix() );
	osg::Vec3 upVector = cameraManipulator->getFrontVector( cameraManipulator->getMatrix() );
	
	// apply the transformation to each axis
	sideVector *= ( dx );
	upVector *= ( dy );
	
	// combine them into the transformation vector
	osg::Vec3 transformVector = sideVector + upVector;
	transformVector.normalize();
	
	
	if(node->getName() == Selection_X_AXIS_NODE_NAME) {
		// shear along x
		s_x += transformVector.x();
	}
	else if(node->getName() == Selection_Y_AXIS_NODE_NAME) {
		// shear along y
		s_y += transformVector.x();
	}
	else if(node->getName() == Selection_Z_AXIS_NODE_NAME) {
		// shear along z
		s_z += transformVector.x();
	}
	
	// set the position
	Selection* selection = dynamic_cast< Selection* >( lastSelected );
	
	if(selection) {
		Model::objRefList selected = view->getModelRef()->getSelection();
		
		if(selected.size() > 0) {
			for( Model::objRefList::iterator i = selected.begin(); i != selected.end(); i++) {
				
			}
		}
	}
	
	return true;
	
}

// translate ("shift") the selector (i.e. if the appropriate key is pressed)
bool selectHandler::shiftSelector( View* viewer, const osgGA::GUIEventAdapter& ea) {
	return false;
}
