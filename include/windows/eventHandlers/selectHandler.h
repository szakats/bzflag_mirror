#ifndef SELECTHANDLER_H_
#define SELECTHANDLER_H_

#include "BZEventHandler.h"
#include "../../render/Selection.h"
#include <osgViewer/Viewer>
#include <osgGA/MatrixManipulator>

#include <string>
#include <map>

using namespace std;

class selectHandler : public BZEventHandler {

public:
	
	// constructor
	selectHandler( View* view, osgGA::MatrixManipulator* baseManipulator );
	~selectHandler() {}
	
	// get the name
	static string getName() { return string("selectHandler"); }
	
	// handle an event
	bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	
	// pick out a Renderable
    bool pickObject(View* viewer, const osgGA::GUIEventAdapter& ea);
    
    // pick the selector
    bool pickSelector( View* viewer, const osgGA::GUIEventAdapter& ea);
    
    // drag the selector
    bool dragSelector( View* viewer, const osgGA::GUIEventAdapter& ea);
    
private:
    // the last Renderable to be selected
    Renderable* lastSelected;
    
    // data associated with the last renderable
    void* lastSelectedData;
    
    // difference in x and y in mouse position (normalized)
    double dx, dy;
    
    // previous x and y values
    double prev_x, prev_y;
    
    // previous event type
    unsigned int prevEvent;
    
    // the base camera manipulator (needed for extracting camera orientation)
    osgGA::MatrixManipulator* cameraManipulator;
};

#endif /*SELECTHANDLER_H_*/
