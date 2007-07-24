#include "../../include/windows/eventHandlers/selectHandler.h"
#include "../../include/render/TextureScalerVisitor.h"

selectHandler::selectHandler( View* view, osgGA::MatrixManipulator* manipulator ) : BZEventHandler( view ) {
	this->lastSelected = NULL;
	this->lastSelectedData = NULL;
	dx = dy = prev_x = prev_y = 0.0;
	this->cameraManipulator = manipulator;
}

// handle an event
bool selectHandler::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa ) {
	View* viewer;
	
    switch(ea.getEventType())
    {
    	
    	// catch drag events
    	case osgGA::GUIEventAdapter::DRAG :
    			
    		viewer = dynamic_cast<View*>(&aa);
    		
    		if( viewer != NULL && this->lastSelected != NULL && this->lastSelected->getName() == Selection_NODE_NAME ) {
    			// if the last event was a DRAG event, we need to update the dx and dy
    			if( this->prevEvent == osgGA::GUIEventAdapter::DRAG ) {
	    			this->dx = ea.getXnormalized() - this->prev_x;
	    			this->dy = ea.getYnormalized() - this->prev_y;
    			}
    			// otherwise, they should be zero
    			else {
    				this->dx = 0;
    				this->dy = 0;	
    			}
    			// set the prev_x and prev_y values so we can re-compute dx and dy on the next event
	    		this->prev_x = ea.getXnormalized();
    			this->prev_y = ea.getYnormalized();
    			
    			// log this event
    			this->prevEvent = osgGA::GUIEventAdapter::DRAG;
    			
    			// handle a selector, based on a pressed key
    			switch( viewer->getKey() ) {
    				// 'r' is for "rotate" (i.e. "spin" in BZW)
    				case BZ_ROTATE_KEY:
    					return this->rotateSelector( view, ea );
    				
    				// 's' is for "scale"
    				case BZ_SCALE_KEY:
    					return this->scaleSelector( view, ea );
    				
    				// 'l' is for "lean" (i.e. "shear" in BZW)
    				case BZ_SHEAR_KEY:
    					return this->shearSelector( view, ea );
    				
    				// 't' is for "translate" (i.e. "shift" in BZW)
    				case BZ_SHIFT_KEY:
    					return this->shiftSelector( view, ea );
    					
    				default:
    					return this->dragSelector( view, ea );
    			}
    		}
    		return false;
    		
    	// catch single-click events (see if we picked the selector or an object)
       	case osgGA::GUIEventAdapter::PUSH : {
       		int button = ea.getButton();
       		
       		if( button == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON ) {
       			viewer = dynamic_cast<View*>(&aa);
       			if( viewer ) {
       				this->prevEvent = osgGA::GUIEventAdapter::PUSH;
       				return this->pickSelector( viewer, ea );
       			}	
       		}
       		else if( button == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON ) {
        		viewer = dynamic_cast<View*>(&aa);
        		if(viewer) {
        			this->prevEvent = osgGA::GUIEventAdapter::PUSH;
        			return this->configureObject(viewer, ea);
        			
        		}	
        	}
       		return false;
       	}
    	// catch double click events and do a pick
        case osgGA::GUIEventAdapter::DOUBLECLICK :
        	if( ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON ) {
	            viewer = dynamic_cast<View*>(&aa);
	            if (viewer) {
	            	this->prevEvent = osgGA::GUIEventAdapter::DOUBLECLICK;
       		
	            	return this->pickObject(viewer,ea);
	            	
	            }
        	}
        	
       		return false;
       		
        default:
            return false;
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
    
    this->lastSelected = NULL;
    this->lastSelectedData = NULL;
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
            		this->lastSelected = obj;
            		
            		// save the node that was picked
            		this->lastSelectedData = pickedNode;
            		
            		return true;
            	}
            }
        }
    }
    
    this->lastSelected = NULL;
    this->lastSelectedData = NULL;
    return false;
}

// see if we picked an object and if so, configure it
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
            	   		this->lastSelected = obj;
            	   		return true;	
            	   	}
            	   	
            	   	return false;
            	}
            }
            
        }
    }
    
    this->lastSelected = NULL;
    this->lastSelectedData = NULL;
    return false;
}

// handle translation events
bool selectHandler::dragSelector( View* viewer, const osgGA::GUIEventAdapter& ea ) {
	osg::Node* node = (osg::Node*)this->lastSelectedData;
	
	// get the position of the last selected object (which should be the axes)
	osg::Vec3 position = this->lastSelected->getPosition();
	
	// transform the 2D mouse movement into a 3D vector by transforming it into camera space
	// get the vectors (but keep in mind that the window uses the XY-plane, but "up" in the 3D scene is along Z)
	osg::Vec3 sideVector = this->cameraManipulator->getSideVector( this->cameraManipulator->getMatrix() );
	osg::Vec3 upVector = this->cameraManipulator->getFrontVector( this->cameraManipulator->getMatrix() );
	
	// apply the transformation to each axis
	sideVector *= ( dx );
	upVector *= ( dy );
	
	// combine them into the transformation vector
	osg::Vec3 transformVector = sideVector + upVector;
	transformVector.normalize();
	
	
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
	Selection* selection = dynamic_cast< Selection* >( this->lastSelected );
	
	// update all objects in the selection
	if(selection) {
		
		// get the model reference from the View
		const Model* model = this->view->getModelRef();
		
		// get the selected objects
		vector< bz2object* > selected = model->getSelection();
		
		// transform them
		if(selected.size() > 0) {
			osg::Vec3 dPosition = position - selection->getPosition();
			osg::Vec3 tmp;
			for(vector<bz2object*>::iterator i = selected.begin(); i != selected.end(); i++) {
				tmp = (*i)->getPosition() + dPosition;
				(*i)->setPosition( tmp );
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
	osg::Node* node = (osg::Node*)this->lastSelectedData;
	
	// get the position of the selector
	osg::Vec3 position = this->lastSelected->getPosition();
	
	// get the angular orientation
	double a_x = 0.0, a_y = 0.0, a_z = 0.0;
	
	// transform the 2D mouse movement into a 3D vector by transforming it into camera space
	// get the vectors (but keep in mind that the window uses the XY-plane, but "up" in the 3D scene is along Z)
	osg::Vec3 sideVector = this->cameraManipulator->getSideVector( this->cameraManipulator->getMatrix() );
	osg::Vec3 upVector = this->cameraManipulator->getFrontVector( this->cameraManipulator->getMatrix() );
	
	// apply the transformation to each axis (only look at movement perpendicular to the axes here, since we're rotating)
	sideVector *= ( dy );
	upVector *= ( -dx );
	
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
	Selection* selection = dynamic_cast< Selection* >( this->lastSelected );
	
	// update all objects in the selection
	if(selection) {
		// get the model reference from the View
		const Model* model = this->view->getModelRef();
		
		// get the selected objects
		vector< bz2object* > selected = model->getSelection();
		
		// transform them
		if(selected.size() > 0) {
			osg::Vec3 dp;
			for(vector<bz2object*>::iterator i = selected.begin(); i != selected.end(); i++) {
				// sort out objects by their rotation technique
				dp = (*i)->getRotation();
				if( (*i)->isKey("rotation") )
					(*i)->setRotationZ( dp.z() + a_z );
				else if( (*i)->isKey("spin") )
					(*i)->setRotation( dp.x() + a_x, dp.y() + a_y, dp.z() + a_z );
			}
			
			// finally, transform the selector itself
			selection->rebuildAxes( selected );
		}
	}
	
	return true;	
}

// handle scale events
bool selectHandler::scaleSelector( View* viewer, const osgGA::GUIEventAdapter& ea ) {
	osg::Node* node = (osg::Node*)this->lastSelectedData;
	
	// get the position of the last selected object (which should be the axes)
	osg::Vec3 scale = osg::Vec3( 0, 0, 0 );
	
	// transform the 2D mouse movement into a 3D vector by transforming it into camera space
	// get the vectors (but keep in mind that the window uses the XY-plane, but "up" in the 3D scene is along Z)
	osg::Vec3 sideVector = this->cameraManipulator->getSideVector( this->cameraManipulator->getMatrix() );
	osg::Vec3 upVector = this->cameraManipulator->getFrontVector( this->cameraManipulator->getMatrix() );
	
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
	Selection* selection = dynamic_cast< Selection* >( this->lastSelected );
	// update all objects in the selection
	
	if(selection) {
		vector<bz2object*> selected = this->view->getModelRef()->getSelection();
		if( selected.size() > 0 ) {
			osg::Vec3 tscale;
			for(vector<bz2object*>::iterator i = selected.begin(); i != selected.end(); i++) {
				tscale = (*i)->getScale() + scale;
				// no negative scaling!
				if( tscale.x() < 0 )
					tscale.set( 0, tscale.y(), tscale.z() );
				if( tscale.y() < 0 )
					tscale.set( tscale.x(), 0, tscale.z() );
				if( tscale.z() < 0 )
					tscale.set( tscale.x(), tscale.y(), 0 );
				// update the scale
				(*i)->setScale( tscale );
				
				TextureScalerVisitor tsv = TextureScalerVisitor( *i, osg::Vec3( 1.0, 1.0, 1.0 ) + scale * 0.04);
				(*i)->accept( tsv );
			}	
		}
	}
	
	return true;	
}

// shear the selector (i.e. if the appropriate key is pressed)
bool selectHandler::shearSelector( View* viewer, const osgGA::GUIEventAdapter& ea) {
	osg::Node* node = (osg::Node*)this->lastSelectedData;
	
	// shear movements
	double s_x = 0.0, s_y = 0.0, s_z = 0.0;
	
	// transform the 2D mouse movement into a 3D vector by transforming it into camera space
	// get the vectors (but keep in mind that the window uses the XY-plane, but "up" in the 3D scene is along Z)
	osg::Vec3 sideVector = this->cameraManipulator->getSideVector( this->cameraManipulator->getMatrix() );
	osg::Vec3 upVector = this->cameraManipulator->getFrontVector( this->cameraManipulator->getMatrix() );
	
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
	Selection* selection = dynamic_cast< Selection* >( this->lastSelected );
	
	if(selection) {
		vector< bz2object* > selected = this->view->getModelRef()->getSelection();
		
		if(selected.size() > 0) {
			for( vector<bz2object*>::iterator i = selected.begin(); i != selected.end(); i++) {
				
			}
		}
	}
	
	return true;
	
}

// translate ("shift") the selector (i.e. if the appropriate key is pressed)
bool selectHandler::shiftSelector( View* viewer, const osgGA::GUIEventAdapter& ea) {
	
}
