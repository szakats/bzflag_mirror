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

#ifndef BZ2OBJECT_H_
#define BZ2OBJECT_H_

#include "render/Renderable.h"
#include "render/Point3D.h"
#include "DataEntry.h"
#include "Transform.h"

class physics;
class material;

#include <vector>
#include <string>

using namespace std;

/**
 * This is the base class of a BZW renderable object.  All of these support name, position, rotation, size, matref, phydrv
 * and sets of shear, shift, scale, and spin key/value pairs.  box and pyramid are derived classes of this, 
 * but simply do not support transformations
 */

class bz2object : public Renderable, public DataEntry
{
	// allow SceneBuilder to modify bz2objects
	friend class SceneBuilder;
	
	public:
	
		// default constructor
		bz2object(const char* name, const char* keys);
		
		// constructor with data
		bz2object(const char* name, const char* keys, const char* data);
		
		// constructor with node
		bz2object( const char* name, const char* keys, osg::Node* node );
		
		// constructor with node and data
		bz2object( const char* name, const char* keys, const char* data, osg::Node* node );
		
		// destructor
		virtual ~bz2object() { }
		
		// getter
		string get(void);
		
		// setter
		virtual int update(string& data);
		
		// specific update message
		virtual int update(UpdateMessage& msg);
		
		// toString
		virtual string toString(void);
		
		// this method only returns the (indented) lines in the BZW text and is meant to be called by derived classes
		string BZWLines(void);
		
		// data getters (makes MasterConfigurationDialog code easier)
		osg::ref_ptr<physics> getPhyDrv() { return physicsDriver; }
		vector< osg::ref_ptr<BZTransform> >& getTransformations() { return transformations; }
		vector< osg::ref_ptr<material> >& getMaterials() { return materials; }
		bool isSelected() { return selected; }
		
		// use this instead of getPosition()
		virtual osg::Vec3 getPos() { return getPosition(); }
		
		// use this instead of getScale()
		virtual osg::Vec3 getSize() { return getScale(); }
		
		// use this instead of getAttitude()
		virtual osg::Quat getRot() { return getAttitude(); }
		
		// use this instead of setPosition()
		virtual void setPos( const osg::Vec3d& newPos ) { 
			startShift->setData( -newPos );
			setPosition( newPos );
			endShift->setData( newPos );
		}
		
		// use this instead of setScale()
		virtual void setSize( const osg::Vec3d& newSize ) { setScale( newSize ); }
		
		// use this instead of setAttitude()
		virtual void setRot( const osg::Quat& newRot ) { setAttitude( newRot ); }
		
		// override Renderable's setRotationX() method
		virtual void setRotationX( float x ) {
			Renderable::setRotationX( x );
			// rotation.set( x, rotation.y(), rotation.z() );
			
			vector<float> spinData;
			spinData.push_back( x );	spinData.push_back(1.0);	spinData.push_back(0.0);	spinData.push_back(0.0);
			spin_x->setData( spinData );
		}
		
		// override Renderable's setRotationY() method
		virtual void setRotationY( float y ) {
			Renderable::setRotationY( y );
			// rotation.set( rotation.x(), y, rotation.z() );
			
			vector<float> spinData;
			spinData.push_back( y );	spinData.push_back(0.0);	spinData.push_back(1.0);	spinData.push_back(0.0);
			spin_y->setData( spinData );
		}
		
		// override Renderable's setRotationZ() method
		virtual void setRotationZ( float z ) {
			Renderable::setRotationZ( z );
			// rotation.set( rotation.x(), rotation.y(), z );
			
			vector<float> spinData;
			spinData.push_back( z );	spinData.push_back(0.0);	spinData.push_back(0.0);	spinData.push_back(1.0);
			
			spin_z->setData( spinData );
		}
		
		// override Renderable's setRotation() method
		virtual void setRotation( float x, float y, float z ) {
			Renderable::setRotation( x, y, z );
			// rotation.set( x, y, z );
			
			vector<float> spinData_x, spinData_y, spinData_z;
			spinData_x.push_back(x);	spinData_x.push_back(1.0);	spinData_x.push_back(0.0);	spinData_x.push_back(0.0);
			spinData_y.push_back(y);	spinData_y.push_back(0.0);	spinData_y.push_back(1.0);	spinData_y.push_back(0.0);
			spinData_z.push_back(z);	spinData_z.push_back(0.0);	spinData_z.push_back(0.0);	spinData_z.push_back(1.0);
			
			spin_x->setData( spinData_x );
			spin_y->setData( spinData_y );
			spin_z->setData( spinData_z );
		}
		virtual void setRotation( const osg::Vec3& rot ) { setRotation( rot.x(), rot.y(), rot.z() ); }
		
		// data setters (makes MasterConfigurationDialog code easier)
		void setPhyDrv( physics* phydrv ) { physicsDriver = phydrv; }
		void setTransforms( vector< osg::ref_ptr<BZTransform> >& _transformations ) { this->transformations = _transformations; }
		void setMaterials( vector< osg::ref_ptr< material > >& _materials ) { this->materials = _materials; }
		void setSelected( bool value ) { selected = value; }
		
		// use this instead of setScale();
		virtual void setSize( osg::Vec3 newSize ) { setScale( newSize ); }
		
		// set/set the thisNode
		osg::Node* getThisNode() { return thisNode.get(); }
		void setThisNode( osg::Node* node ) { 
			endShift->removeChild( thisNode.get() );
			thisNode = node;
			endShift->addChild( thisNode.get() );
		}
		
		// make this public
		bz2object operator =( const bz2object& obj ) { 
			bz2object newObj(NULL, NULL);
			memcpy(&newObj, &obj, sizeof(bz2object));
			return newObj;
		}
		
		// some basic control methods for transformations
		void addTransformation( BZTransform* t );
		void insertTransformation( unsigned int index, BZTransform* t );
		void removeTransformation( BZTransform* t );
		void removeTransformation( unsigned int index );
		
		// some basic control methods for materials
		void addMaterial( material* mat );
		void insertMaterial( unsigned int index, material* mat );
		void removeMaterial( material* mat );
		void removeMaterial( unsigned int index );
		
		
	protected:
		osg::ref_ptr< physics > physicsDriver;
		vector< osg::ref_ptr< material > > materials;
		vector< osg::ref_ptr< BZTransform > > transformations;
		// set true if selected in the 3D scene
		bool selected;
		
		// set the material of this object from the list of materials
		void refreshMaterial();
		
	private:
		// force these methods to be private, to guarantee that derived classes will use the given replacements
		osg::Vec3f getPosition() { return Renderable::getPosition(); }
		osg::Vec3f getScale() { return Renderable::getScale(); }
		osg::Quat getAttitude() { return Renderable::getAttitude(); }
		void setPosition( const osg::Vec3d& newPosition ) { Renderable::setPosition( newPosition ); }
		void setScale( const osg::Vec3d& newScale ) { Renderable::setScale( newScale ); }
		void setAttitude( const osg::Quat& newAttitude ) { Renderable::setAttitude( newAttitude ); }
		
		// start and end shift transformations (manditory), as well as angular orientation transformations
		osg::ref_ptr< BZTransform > startShift, endShift, spin_x, spin_y, spin_z;
		
		// recompute the transformation stack
		void recomputeTransformations( vector< osg::ref_ptr< BZTransform > >* newTransformations);
		
		// reference to node data inside the Renderable (for changing the transformation stack)
		osg::ref_ptr< osg::Node > thisNode;
		
		// saved state set
		osg::ref_ptr< osg::StateSet > savedStateSet;
};

#include "physics.h"
#include "material.h"

#endif /*BZ2OBJECT_H_*/
