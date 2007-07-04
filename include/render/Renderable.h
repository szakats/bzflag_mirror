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
	
	// constructor with object
	Renderable( bz2object* data, osg::CopyOp op = 0) : osg::PositionAttitudeTransform( (data != NULL ? *(data->makeRenderable().get()) : *(new osg::PositionAttitudeTransform())), op ) {
		this->_name = "";
		this->bzObj = data;
	}
	
	// constructor with both name and object
	Renderable( const char* name, bz2object* data, osg::CopyOp op = 0) : osg::PositionAttitudeTransform( (data != NULL ? *(data->makeRenderable().get()) : *(new osg::PositionAttitudeTransform())), op ) {
		this->_name = name;
		this->bzObj = data;	
	}
	
	// constructor with a child node to add
	Renderable( osg::Node* child ) : osg::PositionAttitudeTransform() {
		this->_name = child->getName();
		this->bzObj = NULL;
		
		this->addChild( child );	
	}
	
	// getters
	bz2object* getBZWObject() { return bzObj; }
	osg::ref_ptr< osg::Node > getNode() { return this->getChild( 0 ); }
	
	// setters
	void setBZWObject( bz2object* data ) { this->bzObj = data; }
	// set the child node (there should only be one)
	void setNode( osg::Node* node ) {
		if(this->getChild(0) != NULL)
			this->removeChild( this->getChild(0) );
		
		this->addChild( node );
	}
	
	// angular rotators (easier than building quaterions and setting the attitude
	// angles is in degrees, not radians
	void setRotationX( float rotation ) { this->setAttitude( osg::Quat( osg::DegreesToRadians( rotation ), osg::Vec3( 1.0, 0.0, 0.0 ) ) ); }
	void setRotationY( float rotation ) { this->setAttitude( osg::Quat( osg::DegreesToRadians( rotation ), osg::Vec3( 0.0, 1.0, 0.0 ) ) ); }
	void setRotationZ( float rotation ) { this->setAttitude( osg::Quat( osg::DegreesToRadians( rotation ), osg::Vec3( 0.0, 0.0, 1.0 ) ) ); }
	void setRotation( float rotation, osg::Vec3& axis ) { axis.normalize(); this->setAttitude( osg::Quat( osg::DegreesToRadians( rotation ), axis ) ); }
	
protected:
	
	// bzw object
	bz2object* bzObj;
	
	
};

#endif /*RENDERABLE_H_*/
