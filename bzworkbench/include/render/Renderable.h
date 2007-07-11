#ifndef RENDERABLE_H_
#define RENDERABLE_H_

#include <osg/Group>
#include <osg/PositionAttitudeTransform>
#include <osg/Node>

#include "../objects/bz2object.h"

// this is a subclass of osg::PositionAttitudeTransform.  It is designed to contain a DataEntry's text
// in addition to a renderable OSG node.

class Renderable : public osg::PositionAttitudeTransform {
	
public:
	
	// default constructor
	Renderable() : osg::PositionAttitudeTransform() {
		this->_name = "";
		this->bzObj = NULL;
	}
	
	// constructor with name
	Renderable( const char* name ) : osg::PositionAttitudeTransform() {
		this->_name = name;
		this->bzObj = NULL;	
	}
	
	// constructor with a child node to add
	Renderable( osg::Node* child ) : osg::PositionAttitudeTransform() {
		this->_name = child->getName();
		this->bzObj = NULL;
		
		this->addChild( child );	
	}
	
	// getters
	bz2object* getBZWObject() { return this->bzObj; }
	osg::ref_ptr< osg::Node > getNode() { return this->getChild( 0 ); }
	
	// setters
	void setBZWObject( bz2object* data ) { this->bzObj = data; }
	// set the child node (there should only be one)
	void setNode( osg::Node* node ) {
		if(this->getChild(0) != NULL)
			this->removeChild( this->getChild(0) );
		
		this->addChild( node );
	}
	
	// set the color of the contained node
	void setColor( osg::Vec4 color ) {
			
	}
	
	// angular rotators (easier than building quaterions and setting the attitude
	// angles is in degrees, not radians
	void setRotationX( float r ) {
		this->rotation.set( r, rotation.y(), rotation.z() );
		this->setAttitude( osg::Quat( osg::DegreesToRadians( rotation.x() ), osg::Vec3( 1.0, 0.0, 0.0 ),
									  osg::DegreesToRadians( rotation.y() ), osg::Vec3( 0.0, 1.0, 0.0 ),
									  osg::DegreesToRadians( rotation.z() ), osg::Vec3( 0.0, 0.0, 1.0 ) ) );
	}
	void setRotationY( float r ) {
		this->rotation.set( rotation.x(), r, rotation.z() );
		this->setAttitude( osg::Quat( osg::DegreesToRadians( rotation.x() ), osg::Vec3( 1.0, 0.0, 0.0 ),
									  osg::DegreesToRadians( rotation.y() ), osg::Vec3( 0.0, 1.0, 0.0 ),
									  osg::DegreesToRadians( rotation.z() ), osg::Vec3( 0.0, 0.0, 1.0 ) ) );
	}
	void setRotationZ( float r ) {
		this->rotation.set( rotation.x(), rotation.y(), r );
		this->setAttitude( osg::Quat( osg::DegreesToRadians( rotation.x() ), osg::Vec3( 1.0, 0.0, 0.0 ),
									  osg::DegreesToRadians( rotation.y() ), osg::Vec3( 0.0, 1.0, 0.0 ),
									  osg::DegreesToRadians( rotation.z() ), osg::Vec3( 0.0, 0.0, 1.0 ) ) );
	}
	void setRotation( float x, float y, float z ) {
		this->rotation.set( x, y, z);
		this->setAttitude( osg::Quat( osg::DegreesToRadians( rotation.x() ), osg::Vec3( 1.0, 0.0, 0.0 ),
									  osg::DegreesToRadians( rotation.y() ), osg::Vec3( 0.0, 1.0, 0.0 ),
									  osg::DegreesToRadians( rotation.z() ), osg::Vec3( 0.0, 0.0, 1.0 ) ) );
	}
	
	// getters for rotation
	osg::Vec3 getRotation() { return this->rotation; }
	
protected:
	
	// bzw object
	bz2object* bzObj;
	
	// angular orientation around 3D axes
	osg::Vec3 rotation;
	
};

#endif /*RENDERABLE_H_*/
