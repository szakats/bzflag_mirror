#include "../include/objects/box.h"

// constructors
box::box() : bz2object("box", "<position><rotation><size>") {
	
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

// get renderable
osg::ref_ptr< osg::Node > box::getRenderable() {
	osg::PositionAttitudeTransform* boxTransform = SceneBuilder::transformable( SceneBuilder::buildNode( "share/box/box.obj" ).get() ).get();
	
	boxTransform->setPosition( osg::Vec3(this->position.x, this->position.y, this->position.z + this->size.z / 2.0) );
	boxTransform->setScale( osg::Vec3(this->size.x, this->size.y, this->size.z / 2.0) );
	boxTransform->setAttitude( osg::Quat( osg::DegreesToRadians( this->rotation ), osg::Vec3( 0.0, 0.0, 1.0 ) ) );
	
	return osg::ref_ptr<osg::Node> (boxTransform);
}
