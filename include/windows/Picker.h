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

#include "View.h"

using namespace std;

// class to handle object selection with the mouse.
// default selection gesture is a double-click.
// default return value of pick() is the nearest object to the camera that was clicked
class Picker : public osgGA::GUIEventHandler {
public: 

    Picker( View* view ) { this->view = view; }
    ~Picker() {}
    
    virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

	// pick out a PositionAttitudeTransform from the scene (which will contain the node(s) we desire)
    virtual osg::PositionAttitudeTransform* pick(osgViewer::Viewer* viewer, const osgGA::GUIEventAdapter& ea);
   
private:
	View* view;
};

#endif /*PICKER_H_*/
