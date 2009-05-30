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

#ifndef UPDATEMESSAGE_H_
#define UPDATEMESSAGE_H_

#include <osg/Vec3>
#include <osg/Quat>
#include <osg/ref_ptr>

#include <vector>

using namespace std;

/**
 * This is a simple message class that can be sent to objects in their update() method.
 * 
 * NOTE: the intended use of this messaging class is to inform an object that it was changed externally.
 * It's the job of the update() method to interpret the message and carry out any additional changes
 * specific to the object itself.
 */
 
class BZTransform;
class material;
 
class UpdateMessage {
	
public:

	// built-in types for complex message structures
	typedef struct {
		BZTransform* transform;
		unsigned int index;	
	} IndexedTransform;	// transformation to be inserted
	
	typedef struct {
		material* mat;
		unsigned int index;
	} IndexedMaterial;	// material to be inserted

	// some common message types
	enum UpdateMessageType {
		SET_POSITION,				// i.e. use this to indicate a new position
		SET_POSITION_FACTOR,		// i.e. use this to indicate a translation
		SET_ROTATION,				// i.e. use this to indicate a new rotation
		SET_ROTATION_FACTOR,		// i.e. use this to indicate a change in rotation
		SET_SCALE,					// i.e. use this to indicate a new scale
		SET_SCALE_FACTOR,			// i.e. use this to indicate a change in scale
		
		ADD_TRANSFOMRATION,			// i.e. push a transformation
		REMOVE_TRANSFORMATION,		// i.e. pop a transformation
		UPDATE_TRANSFORMATION,		// i.e. change a transformation
		INSERT_TRANSFORMATION,		// i.e. insert a transformation at a given index
		SET_TRANSFORMATIONS,		// i.e. use this along with a vector of BZTransforms to set the transformation stack
		
		ADD_MATERIAL,				// i.e. push a material to the object's material list
		REMOVE_MATERIAL,			// i.e. pop a material from the object's material list
		UPDATE_MATERIAL,			// i.e. change a material
		INSERT_MATERIAL,			// i.e. insert a material at a given index
		SET_MATERIALS,				// i.e. set the entire list of materials
		
		SET_PHYDRV,					// i.e. set the physics driver
		
		USER_EVENT					// i.e. user-defined event
	};

	int type;
	
	/*
	UpdateMessage( int type, void* data) {
		this->type = type;
		this->data = data;
	}
	*/
	UpdateMessage( int _type, osg::Vec3* _data ) {
		this->type = _type;
		this->data.vec = _data;
	}
	
	UpdateMessage( int _type, osg::Quat* _data ) {
		this->type = _type;
		this->data.quat = _data;
	}
	
	UpdateMessage( int _type, BZTransform* _data ) {
		this->type = _type;
		this->data.transform = _data;
	}
	
	UpdateMessage( int _type, IndexedTransform* _data ) {
		this->type = _type;
		this->data.indexedTransformation = _data;	
	}
	
	UpdateMessage( int _type, IndexedMaterial* _data ) {
		this->type = _type;
		this->data.indexedMaterial = _data;	
	}
	
	UpdateMessage( int _type, vector< osg::ref_ptr<BZTransform> >* _data) {
		this->type = _type;
		this->data.transforms = _data;
	}
	
	UpdateMessage( int _type, material* _data ) {
		this->type = _type;
		this->data.mat = _data;
	}
	
	UpdateMessage( int _type, vector< material* >* _data ) {
		this->type = _type;
		this->data.materials = _data;
	}
	
	// message-specific getters (ensure type-safety)
	osg::Vec3* getAsPosition() { return data.vec; }
	osg::Vec3* getAsPositionFactor() { return data.vec; }
	osg::Vec3* getAsScale() { return data.vec; }
	osg::Vec3* getAsScaleFactor() { return data.vec; }
	osg::Quat* getAsAttitude() { return data.quat; }
	osg::Vec3* getAsRotation() { return data.vec; }
	osg::Vec3* getAsRotationFactor() { return data.vec; }
	
	BZTransform* getAsTransformation() { return data.transform; }
	vector< osg::ref_ptr<BZTransform> >* getAsTransformationStack() { return data.transforms; }
	IndexedTransform* getAsIndexedTransform() { return data.indexedTransformation; }
	
	material* getAsMaterial() { return data.mat; }
	vector< material* >* getAsMaterialList() { return data.materials; }
	IndexedMaterial* getAsIndexedMaterial() { return data.indexedMaterial; }
	
private:
	
	union {
		osg::Vec3* vec;		// position (factor), scale (factor), and rotation (factor) data
		osg::Quat* quat;	// rotation (factor) data
		
		BZTransform* transform;		// transformation to be pushed/popped
		
		IndexedTransform* indexedTransformation;	// transformation to be inserted
		
		vector< osg::ref_ptr<BZTransform> >* transforms;	// transformations
		
		material* mat;			// a material to be pushed/popped
		vector< material* >* materials;		// material stack
		
		IndexedMaterial* indexedMaterial; 		// material to be inserted
		
	} data;
	
};

#endif /*UPDATEMESSAGE_H_*/
