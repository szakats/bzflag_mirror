#include "../include/objects/pyramid.h"

pyramid::pyramid() : bz2object("pyramid", "<position><rotation><size>") {
	this->addChild( SceneBuilder::buildNode("share/pyramid/pyramid.obj") );
	
	this->setName( SceneBuilder::nameNode("share/pyramid/pyramid.obj") );
	
	this->setPosition( osg::Vec3(0.0, 0.0, 0.0) );
	this->setScale( osg::Vec3(10.0, 10.0, 10.0) );
	SceneBuilder::markUnselected( this );
}

// constructor with binary data
pyramid::pyramid( osg::Vec3 position, float rotation, osg::Vec3 scale ) : bz2object("pyramid", "<position><rotation><size>") {
	this->addChild( SceneBuilder::buildNode("share/pyramid/pyramid.obj") );
	
	this->setName( SceneBuilder::nameNode("share/pyramid/pyramid.obj") );
	
	this->setPosition( position );
	this->setRotationZ( rotation );
	this->setScale( scale );
	SceneBuilder::markUnselected( this );
}

// constructor with string
pyramid::pyramid(string& data) : bz2object("pyramid", "<position><rotation><size>", data.c_str()) {
	this->addChild( SceneBuilder::buildNode("share/pyramid/pyramid.obj") );
	this->setName( SceneBuilder::nameNode("share/pyramid/pyramid.obj") );
	SceneBuilder::markUnselected( this );
	
	if( data.length() <= 1 ) {
		this->setPosition( osg::Vec3(0.0, 0.0, 0.0) );
		this->setScale( osg::Vec3(10.0, 10.0, 10.0) );
	}
	else 
		this->update(data);	
}	

// getter
string pyramid::get() { return this->toString(); }

// setter
int pyramid::update(string& data) {
	return bz2object::update(data);	
}

// toString
string pyramid::toString(void) {
	return string("pyramid\n") +
				  this->BZWLines() +
				  "end\n";	
}

// render
int pyramid::render(void) {
	return 0;
}
