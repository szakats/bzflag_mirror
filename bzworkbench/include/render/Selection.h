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
#include "Renderable.h"
#include "../model/SceneBuilder.h"

using namespace std;

#define Selection_NODE_NAME 			"selector3D"
#define Selection_CENTER_NODE_NAME		"selector3D_center"
#define Selection_X_AXIS_NODE_NAME		"selector3D_x_axis"
#define Selection_Y_AXIS_NODE_NAME		"selector3D_y_axis"
#define Selection_Z_AXIS_NODE_NAME		"selector3D_z_axis"

/**
 * A selection is just a collection of selected Renderables.
 * This object will render as a set of axes, like in Blender.
 */

class Selection : public Renderable {

public:

	// constructer
	Selection();
	Selection( vector<Renderable*>& selection );
	
	// destructor
	~Selection() {}
	
	// add an object to the selection
	void add( Renderable* r );
	
	// remove from a selection
	void remove( Renderable* r );
	
	// is selected
	bool contains( Renderable* r );
	
	// remove all
	void removeAll();
	
	// get the selected
	map<Renderable*, Renderable*> getSelection() { return this->selected; }
	
	// called by selectHandler to determine which part of the selector was picked
	static osg::Node* getPickedNode( Renderable* r, const osg::NodePath& pickedNodes, unsigned int startIndex = 0 );
	
	// refresh (i.e. recompute center)
	void refresh() { this->rebuildAxes(); }
	
private:

	// some constants defining the shape and color of the axes
	static const float CENTER_RADIUS = 2.0f;
	static const float SHAFT_LENGTH = 10.0f;
	static const float SHAFT_RADIUS = 0.5f;
	static const float TIP_LENGTH = 3.0f;
	static const float TIP_RADIUS = 1.0f;
	
	// the selected renderables (really a reflexive map)
	map< Renderable*, Renderable* > selected;
	
	// compute the local origin of the selection
	osg::Vec3 computeLocalOrigin();
	
	// build the axes geode
	osg::ref_ptr< Renderable > buildAxes( osg::Vec3 localOrigin );
	
	// store the 3-axis geode
	osg::ref_ptr< Renderable > axes;
	
	// regenerate the axes
	void rebuildAxes();
	
};

#endif /*SELECTION_H_*/
