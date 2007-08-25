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

#include "../include/objects/group.h"

// constructor
group::group() : 
	bz2object("group", "<shift><shear><scale><spin><team><tint><drivethrough><shootthrough><phydrv><matref>") {
	this->team = 0;
	this->def = NULL;
	this->tintColor = RGBA(1, 1, 1, 1);
	this->driveThrough = false;
	this->shootThrough = false;
	this->setName("");
	this->setPos( osg::Vec3( 0.0, 0.0, 0.0 ) );
	
	this->container = new Renderable();
	this->geoRing = NULL;
}

// constructor with data
group::group(string& data) : 
	bz2object("group", "<shift><shear><scale><spin><team><tint><drivethrough><shootthrough><phydrv><matref>", data.c_str()) {
	this->team = 0;
	this->def = NULL;
	this->tintColor = RGBA(1, 1, 1, 1);
	this->driveThrough = false;
	this->shootThrough = false;
	this->setName("");
	this->setPos( osg::Vec3( 0.0, 0.0, 0.0 ) );
	
	this->container = new Renderable();
	this->geoRing = NULL;
	
	this->update(data);
}

// getter
string group::get(void) {
	return this->toString(); 
}

// setter
int group::update(string& data) {
	const char* header = this->getHeader().c_str();
	
	// get the section from the data
	const vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
	
	if(lines[0] == BZW_NOT_FOUND)
		return 0;
		
	if(!hasOnlyOne(lines, "group"))
		return 0;
	
	const char* groupData = lines[0].c_str();
	
	// get name (from the first line)
	vector<string> headers = BZWParser::getValuesByKey("group", header, groupData);
	
	// get tint
	vector<string> tints = BZWParser::getValuesByKey("tint", header, groupData);
	if(tints.size() > 1) {
		printf("group::update(): Error! Defined \"tint\" %d times\n", tints.size());
		return 0;	
	}
	
	// get team
	vector<string> teams = BZWParser::getValuesByKey("team", header, groupData);
	if(teams.size() > 1) {
		printf("group::update(): Error! Defined \"team\" %d times\n", tints.size());
		return 0;	
	}
	
	// get drivethrough
	vector<string> driveThroughs = BZWParser::getValuesByKey("drivethrough", header, groupData);
		
	// get shootthrough
	vector<string> shootThroughs = BZWParser::getValuesByKey("shootthrough", header, groupData);
			
	// do base class update
	if(!bz2object::update(data))
		return 0;
		
	// the superclass bz2object will apply "spin" transformations if present.  These need to be forewarded
	// to the container object, and removed from the group for correct rendering
	osg::Vec3 rot = this->getRotation();
	if( this->container.get() != NULL )
		this->container->setRotation( rot );
		
	this->setRotation( osg::Vec3( 0, 0, 0 ) );
	
	// assign data
	// see if the name changed (if so, recompute the object list)
	string oldName = this->getName();
	this->setName( headers[0] );
	if( oldName != this->getName() )
		this->updateObjects();
		
	this->tintColor = (tints.size() > 0 ? RGBA( tints[0].c_str() ) : RGBA(-1, -1, -1, -1));
	this->team = (teams.size() > 0 ? (int)(atof( teams[0].c_str() )) : -1);
	this->driveThrough = (driveThroughs.size() == 0 ? false : true);
	this->shootThrough = (shootThroughs.size() == 0 ? false : true);
	
	return 1;
	
}

// event handler
int group::update( UpdateMessage& message ) {
	
	// superclass update (i.e. handle transformation changes)
	int result = bz2object::update( message );
	
	// make sure we keep the Euler "spin" transformations 0 in the group
	// and forward any existing ones to the container node
	osg::Vec3 rot = this->getRotation();
	if( this->container.get() != NULL )
		this->container->setRotation( rot );
		
	this->setRotation( osg::Vec3( 0, 0, 0 ) );
	
	// NOW handle the messages
	switch( message.type ) {
		
		case UpdateMessage::SET_POSITION: {
			this->setPos( *(message.getAsPosition()) );
			
			break;
		}
			
		case UpdateMessage::SET_POSITION_FACTOR: {	// handle a translation
			this->setPos( this->getPos() + *(message.getAsPositionFactor()) );
			
			break;
		}
		case UpdateMessage::SET_ROTATION:		// handle a new rotation
			// propogate rotation events to the children objects
			this->container->setRotation( *(message.getAsRotation()) );
			this->buildGeometry();
			
			break;
			
		case UpdateMessage::SET_ROTATION_FACTOR:	// handle an angular translation
			this->container->setRotation( *(message.getAsRotationFactor()) + this->container->getRotation()  );
			this->buildGeometry();
			
			break;
			
		case UpdateMessage::SET_SCALE:		// handle a new scale
			this->container->setScale( *(message.getAsScale()) );
			this->buildGeometry();
			
			break;
			
		case UpdateMessage::SET_SCALE_FACTOR:	// handle a scaling factor
			this->container->setScale( *(message.getAsScaleFactor()) + this->container->getScale() );
			this->buildGeometry();
			
			break;
			
		default:	// unknown event; don't handle
			return result;
	}
	
	return 1;	
}

// toString
string group::toString(void) {
	osg::Vec3 p = this->getPos();
	string tintString = string(""), teamString = string("");
	if(tintColor.r() > 0 && tintColor.g() > 0 && tintColor.b() > 0 && tintColor.a() > 0)
		tintString = "  tint " + tintColor.toString();
	if(team > 0)
		teamString = "  team " + string(itoa(team)) + "\n";
	
	// temporarily set the rotation of the root group node from the container child so BZWLines() translates
	// the rotation values into "spin" lines
	this->setRotation( this->container->getRotation() );
	
	string ret = string("group ") + this->getName() + "\n" +
				  tintString + 
				  teamString +
				  (driveThrough == true ? "  drivethrough\n" : "") +
				  (shootThrough == true ? "  shootThrough\n" : "") +
				  this->BZWLines() +
				  "end\n";
				  
	// reset the rotation to 0 (so only the container has the spin transformations)
	this->setRotation( osg::Vec3( 0.0, 0.0, 0.0 ) );
	
	// return the string data
	return ret;
}

// build the ring geometry around the objects
void group::buildGeometry() {
	// compute the maximum radius outside the center
	float maxRadius2 = 25.0f;	// radius squared (saves sqrt() calls)
	float maxDim = 0.0f;
	if( this->container->getNumChildren() > 0 ) {
		// get each child
		for( unsigned int i = 0; i < this->container->getNumChildren(); i++ ) {
			osg::Node* child = this->container->getChild( i );
			bz2object* obj = dynamic_cast< bz2object* >(child);
			if( obj ) {
				// only count bz2object instances
				osg::Vec3 p = obj->getPos();
				osg::Vec3 size = obj->getSize();
				
				// compute the largest dimension
				float maxSizeDim = max( size.x(), max( size.y(), size.z() ) );
				maxDim = max( maxSizeDim, maxDim );
				
				// the group's position relative to the objects is (0,0,0), so just square the position and take the length
				float len2 = p.x()*p.x() + p.y()*p.y() + p.z()*p.z();
				
				// see if it's bigger than the maximum radius
				if( len2 > maxRadius2 ) {
					maxRadius2 = len2;
				}
			}
		}
	}
	
	// now find the maximum radius
	float maxRadius = sqrt( maxRadius2 ) + maxDim;
	
	// NOW we can build the geometry
	osg::Vec3Array* points = new osg::Vec3Array();
	
	// primitive set
	osg::DrawElementsUInt* primitives = new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLE_STRIP, 0 );
	
	int index = 0;
	// increment by degrees (less float-point error)
	for( float angle = 0.0f; angle < 360.0f; angle += 5.0f, index += 2 ) {
		float radAngle = osg::DegreesToRadians( angle );
		
		// add mesh data
		points->push_back( osg::Vec3( maxRadius * cos( radAngle ), maxRadius * sin( radAngle ), this->getPos().z() ));
		points->push_back( osg::Vec3( maxRadius * cos( radAngle ), maxRadius * sin( radAngle ), this->getPos().z() + 3 ));
	
		// add primitive indexes
		primitives->push_back( index );
		primitives->push_back( index + 1 );
	}
	
	// make it loop back
	primitives->push_back( 0 );
	primitives->push_back( 1 );
	
	// the geometry node
	this->geoRing = new osg::Geode();
	
	// build that geode!
	osg::Geometry* geometry = new osg::Geometry();
	geometry->setVertexArray( points );
	geometry->addPrimitiveSet( primitives );
	geoRing->addDrawable( geometry );
	
	// just name it
	geoRing->setName("group_container");
	
	// make it fushia
	SceneBuilder::assignMaterial( osg::Vec4( 1.0, 0.0, 1.0, 1.0 ),
								  osg::Vec4( 1.0, 0.0, 1.0, 1.0 ),
								  osg::Vec4( 1.0, 0.0, 1.0, 1.0 ),
								  osg::Vec4( 0.0, 0.0, 0.0, 0.0 ),
								  0.0,
								  1.0,
								  geoRing.get(),
								  osg::StateAttribute::OVERRIDE );
	
	// remove it if it already exists
	if( this->containsNode( geoRing.get() ) && geoRing.get() != NULL )
		this->removeChild( geoRing.get() );
	
	this->addChild( geoRing.get() );
}

// re-compute the list of objects contained in the group
void group::updateObjects() {
	
	// get the "define" reference
	define* def = dynamic_cast< define* >(Model::command( MODEL_GET, "define", this->getName() ));
	
	// reload the children
	setDefine( def );
	
	// update the geometry
	buildGeometry();
}

// set the associated definition
void group::setDefine( define* def ) {
	this->def = def;
	setName( def->getName() ); 
	
	// reload the children
	computeChildren();
	
	// recompute the geometry
	this->buildGeometry();
}

// set the children
void group::computeChildren() {
	// remove all current objects
	if( this->container->getNumChildren() > 0 )
		this->container->removeChildren(0, this->container->getNumChildren());
	
	// if the def is valid, add the objects
	if( def != NULL ) {
		this->def = def;
		// get the objects
		vector< osg::ref_ptr< bz2object > > objects = def->getObjects();
		// put each object inside a PositionAttitudeTransform
		// add them as children of this object
		if( objects.size() > 0 ) {	
			// first, compute the group's center
			float x = 0.0f, y = 0.0f, z = 0.0f;
			for( vector< osg::ref_ptr< bz2object > >::iterator i = objects.begin(); i != objects.end(); i++ ) {
				x += i->get()->getPos().x();
				y += i->get()->getPos().y();
				z += i->get()->getPos().z();
			}
			
			x /= objects.size();
			y /= objects.size();
			z /= objects.size();
			
			osg::Vec3 position = osg::Vec3( x, y, z );
			this->setPos( position );
			
			for( vector< osg::ref_ptr< bz2object > >::iterator i = objects.begin(); i != objects.end(); i++ ) {
				this->container->addChild( i->get() );
				i->get()->setPos( i->get()->getPos() - position );
				
				printf(" added %s\n", (*i)->getName().c_str() );
			}
		}
	}
	
	if( !this->containsNode( container.get() ) )
		this->addChild( container.get() );
}
