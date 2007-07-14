#ifndef SELECTION_H_
#define SELECTION_H_

#include <osg/Group>
#include <osg/Geode>
#include <osg/Node>
#include <osg/ShapeDrawable>
#include <osg/Shape>

#include <map>
#include <string>

#include <GL/gl.h>

#include "Point3D.h"
#include "../objects/bz2object.h"
#include "../model/SceneBuilder.h"
#include "../model/Model.h"
#include "../Observer.h"
#include "../Observable.h"

using namespace std;

#define Selection_NODE_NAME 			"selector3D"
#define Selection_CENTER_NODE_NAME		"selector3D_center"
#define Selection_X_AXIS_NODE_NAME		"selector3D_x_axis"
#define Selection_Y_AXIS_NODE_NAME		"selector3D_y_axis"
#define Selection_Z_AXIS_NODE_NAME		"selector3D_z_axis"

/**
 * A selection is just a collection of selected Renderables.
 * This object will render as a set of axes, like in Blender.
 * This class is also an observer--it watches the model for changes
 * in object selection.
 */

class Selection : public Renderable, public Observer {

public:

	// constructer
	Selection();
	
	// destructor
	~Selection() {}
	
	// called by selectHandler to determine which part of the selector was picked
	static osg::Node* getPickedNode( Renderable* r, const osg::NodePath& pickedNodes, unsigned int startIndex = 0 );
	
	// inherited update() method
	void update( Observable* observable, void* data );
	
	// regenerate the axes
	void rebuildAxes( vector< bz2object* >& objects );
	
private:

	// some constants defining the shape and color of the axes
	static const float CENTER_RADIUS;
	static const float SHAFT_LENGTH;
	static const float SHAFT_RADIUS;
	static const float TIP_LENGTH;
	static const float TIP_RADIUS;
	
	// build the axes geode
	osg::ref_ptr< Renderable > buildAxes( osg::Vec3 localOrigin );
	
	// store the 3-axis geode (i.e. flyweight it)
	osg::ref_ptr< Renderable > axes;
	
	// compute the local origin
	osg::Vec3 computeLocalOrigin( vector< bz2object* >& objects );
	
};
	
#endif /*SELECTION_H_*/
