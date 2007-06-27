#include "../include/objects/box.h"

// constructors
box::box() : bz2object("box", "<position><rotation><size>") {
	this->position = Point3D( 0.0, 0.0, 0.0 );
	this->size = Point3D( 10.0, 10.0, 10.0 );
}

box::box(string& data) : bz2object("box", "<position><rotation><size>", data.c_str()) {
	this->update(data);	
}

// nothing to destroy...
box::~box() { }

// getter
string box::get(void) {
	return this->toString();
}

// setter
int box::update(string& data) {
	return bz2object::update(data);		
}

// toString
string box::toString(void) {
	return string("box\n") +
				  this->BZWLines() +
				  "end\n";
}

// get renderable instance (will be invalid upon changing anything)
osg::ref_ptr< osg::PositionAttitudeTransform > box::getRenderable(void) {
	osg::ref_ptr<osg::PositionAttitudeTransform> boxTransform = SceneBuilder::transformable( SceneBuilder::buildNode( "share/box/box.obj" ).get() );
	
	boxTransform->setPosition( osg::Vec3( this->getPosition()->x(), this->getPosition()->y(), this->getPosition()->z() + this->getSize()->z() / 2.0 ) );
	boxTransform->setScale( osg::Vec3( this->getSize()->x(), this->getSize()->y(), this->getSize()->z() / 2.0 ) );
	boxTransform->setAttitude( osg::Quat( osg::DegreesToRadians( this->rotation ), osg::Vec3( 0.0, 0.0, 1.0 ) ) );
	
	return boxTransform;
}
