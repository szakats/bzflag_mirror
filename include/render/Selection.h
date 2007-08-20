/* BZWorkbench
 * Copyright (c) 1993 - 2007 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef SELECTION_H_
#define SELECTION_H_

#include <osg/Group>
#include <osg/Geode>
#include <osg/Node>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osg/TexEnv>

#include <map>
#include <string>

#include <GL/gl.h>

#include "Point3D.h"
#include "../objects/bz2object.h"
#include "../model/SceneBuilder.h"
#include "../model/Model.h"
#include "../Observer.h"
#include "../Observable.h"
#include "../defines.h"

using namespace std;

// each selector geode has 3 controls--one for the x axis, one for the y axis, and one for the z axis.
// the program identifies the sub-geometries for those controls by these names.

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

	// states of the 3D cursor
	typedef enum SelectionState {
		TRANSLATE,
		ROTATE,
		SCALE,
		SHIFT,
		SHEAR
	};

	// constructer
	Selection(SelectionState state = TRANSLATE);
	
	// destructor
	~Selection() {}
	
	// called by selectHandler to determine which part of the selector was picked
	static osg::Node* getPickedNode( Renderable* r, const osg::NodePath& pickedNodes, unsigned int startIndex = 0 );
	
	// inherited update() method
	void update( Observable* observable, void* data );
	
	// regenerate the axes
	void rebuildAxes( vector< bz2object* >& objects );
	
	// set the state
	SelectionState setState( SelectionState state );
	SelectionState setStateByKey( unsigned char c );
	
private:

	// some constants defining the shape and color of the axes
	static const float CENTER_RADIUS;
	static const float SHAFT_LENGTH;
	static const float SHAFT_RADIUS;
	static const float TIP_LENGTH;
	static const float TIP_RADIUS;
	
	// the state of the handler (translate, rotate, scale, etc.)
	SelectionState state;
	
	// build the axes geode
	osg::ref_ptr< Renderable > buildAxes( osg::Vec3 localOrigin );
	
	// build the scaler geode
	osg::ref_ptr< Renderable > buildScaler( osg::Vec3 localOrigin );
	
	// build the rotator geode
	osg::ref_ptr< Renderable > buildRotator( osg::Vec3 localOrigin );
	
	// build a copy of the local object axes
	osg::ref_ptr< Renderable > buildLocalAxes( osg::Vec3 localOrigin );
	
	// the current node
	osg::ref_ptr< Renderable > selectionNode;
	
	// store the 3-axis geode (i.e. flyweight it)
	osg::ref_ptr< Renderable > axes;
	
	// store the scale selector
	osg::ref_ptr< Renderable > scaler;
	
	// store the rotate selector
	osg::ref_ptr< Renderable > rotator;
	
	// local axis copy
	osg::ref_ptr< Renderable > objectAxes;
	
	// local axis group
	osg::ref_ptr< Renderable > objectAxisGroup;
	
	// compute the local origin
	osg::Vec3 computeLocalOrigin( vector< bz2object* >& objects );
	
};
	
#endif /*SELECTION_H_*/
