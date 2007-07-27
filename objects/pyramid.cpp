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

// setter with messaging
int pyramid::update(string& data, UpdateMessage& message) {
	
	// do the bz2object update
	int result = bz2object::update(data);
	
	// bail if the update failed
	if(result == 0) {
		printf(" update failure\n");
		return result;
	}
	
	// if we changed the scale, update the texture coordinates (i.e. the scale might have changed)
	// NOTE: it is expected that message.data will point to an osg::Vec3, which contains the scaling FACTOR
	if( message.type == UpdateMessage::SET_SCALE ) {
		
		// extract the scale factor from the message
		osg::Vec3* scaleFactor = (osg::Vec3*)message.data;
		
		// get the geometries from the box mesh
		GeometryExtractorVisitor geoExtractor = GeometryExtractorVisitor( this );
		vector< osg::Geometry* > geos = geoExtractor.getGeometries();
		
		// there should be 2 geometries (One Geometry makes up the 4 sides, the other the floor.)
		// if there isn't, then bail
		if( geos.size() != 2 ) {
			printf(" error! %d geometries (expected 2)\n", geos.size());
			return result;
		}
		
		
		
	}
	
	return result;
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
