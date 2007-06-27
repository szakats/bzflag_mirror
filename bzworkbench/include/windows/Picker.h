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
#include <osg/io_utils>

// class to handle events with a pick
class Picker : public osgGA::GUIEventHandler {
public: 

    Picker() {}
    ~Picker() {}
    
    bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

    virtual void pick(osgViewer::Viewer* viewer, const osgGA::GUIEventAdapter& ea);

};

#endif /*PICKER_H_*/
