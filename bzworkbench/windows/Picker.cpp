#include "../include/windows/Picker.h"

// the event handler (takes events from OSG GuiAdapters)
// just pick them; don't eliminate the event from the queue
bool Picker::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
    switch(ea.getEventType())
    {
    	// catch double click events
        case(osgGA::GUIEventAdapter::DOUBLECLICK):
        {
            osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
            if (viewer)
            	this->pick(viewer,ea);
            return false;
        }    
        default:
            return false;
    }
}

// use the OSG intersection API to pick objects
osg::PositionAttitudeTransform* Picker::pick(osgViewer::Viewer* viewer, const osgGA::GUIEventAdapter& ea) {
	// intersections with the scene
    osgUtil::LineSegmentIntersector::Intersections intersections;
	
	// get the intersections from the point in the view where we clicked
    if(viewer->computeIntersections( ea.getX(), ea.getY(), intersections ) ) {
    	// iterate through the intersections
    	printf("-----------------------------\n");
        for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin(); hitr != intersections.end(); ++hitr) {
            
            if(hitr->nodePath.empty())
            	continue;
            
            printf("intersected %d nodes\n", hitr->nodePath.size());
            for(unsigned int i = 0; i < hitr->nodePath.size(); i++) {
            	// only look for PositionAttitudeTransform nodes
            	osg::PositionAttitudeTransform* obj = dynamic_cast< osg::PositionAttitudeTransform* > ( hitr->nodePath[i] );
            	if(obj != NULL && obj->getName().length() > 0) {
            		printf("node %d: %s\n", i, hitr->nodePath[i]->getName().c_str());
            		this->view->handlePicker( this, obj );
            		return obj; 
            	}
            }
            /*
            else if (hitr->drawable.valid()) {
                
            }
			*/
            const osgUtil::LineSegmentIntersector::Intersection::IndexList& vil = hitr->indexList;
            
        }
    }
    return NULL;
}
