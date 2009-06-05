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

#include "objects/cone.h"

#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/PrimitiveSet>
#include <osg/StateSet>
#include <osg/StateAttribute>
#include <osg/ShadeModel>

const char* cone::sideNames[MaterialCount] = {
  "edge",
  "bottom",
  "startside",
  "endside"
};

// default constructor
cone::cone() :
	bz2object("cone", "<position><rotation><size><angle><flatshading><name><texsize><flipz><divisions><shift><shear><spin><scale><smoothbounce><phydrv><matref>") {
	setDefaults();
}

// constructor with data
cone::cone(string& data) :
	bz2object("cone", "<position><rotation><size><angle><flatshading><name><texsize><flipz><divisions><shift><shear><spin><scale><smoothbounce><phydrv><matref>") {
	setDefaults();

	update(data);
}

void cone::setDefaults() {
	// define some basic values
	divisions = 16;
	physicsDriver = NULL;
	flatShading = false;
	smoothbounce = true;
	pyramidStyle = false;

	sweepAngle = 360.0f;

	// make group and geodes
	osg::Group* group = new osg::Group();
	for (int i = 0; i < 4; i++)
		group->addChild( new osg::Geode() );
	setThisNode( group );

	// assign default textures
	SceneBuilder::assignTexture( "boxwall", group->getChild( 0 ) );
	SceneBuilder::assignTexture( "roof", group->getChild( 1 ) );
	SceneBuilder::assignTexture( "wall", group->getChild( 2 ) );
	SceneBuilder::assignTexture( "wall", group->getChild( 3 ) );

	// default size is 10x10x10
	setSize( osg::Vec3( 10, 10, 10 ) );

	// build the geometry
	buildGeometry();
}

// getter
string cone::get(void) { return toString(); }

// setter
int cone::update(string& data) {
	const char* header = getHeader().c_str();
	// get the chunk we need
	vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str(), "end");

	// check and see if the proper data segment was found
	if(lines[0] == BZW_NOT_FOUND) {
		printf("cone: data not found\n");
		return 0;
	}

	string key = (pyramidStyle == true ? "meshpyr" : "cone");
	if(!hasOnlyOne(lines, key.c_str())) {
		printf("cone: improper data\n");
		return 0;
	}

	const char* coneData = lines[0].c_str();

	// get the matrefs
	osg::Group* group = (osg::Group*)getThisNode();
	for (int i = 0; i < MaterialCount; i++) {
		vector<string> faces;
		faces.push_back(sideNames[i]);

		vector<string> matrefs = BZWParser::getValuesByKeyAndFaces("matref", faces, header, coneData);

		if (matrefs.size() > 0) {
			vector< material* > materials;
			for (vector<string>::iterator itr = matrefs.begin(); itr != matrefs.end(); itr++) {
				material* mat = (material*)Model::command( MODEL_GET, "material", *itr );
				if (mat != NULL)
					materials.push_back( mat );
				else
					printf("cone::update(): Error! Couldn't find material %s\n", (*itr).c_str());
			}

			material* finalMat = material::computeFinalMaterial(materials);

			group->getChild(i)->setStateSet(finalMat);
		}
	}

	// get the texsize
	vector<string> texsizes = BZWParser::getValuesByKey("texsize", header, coneData);
	if(texsizes.size() > 1) {
		printf("cone::update(): Error! Defined \"texsize\" %d times!\n", (int)texsizes.size());
		return 0;
	}

	// get the divisions
	vector<string> vDivisions = BZWParser::getValuesByKey("divisions", header, coneData);
	if( vDivisions.size() > 1 ) {
		printf("cone::update(): error! defined \"divisions\" %d times\n", (int)vDivisions.size() );
		return 0;
	}
	if( vDivisions.size() == 0 )
		vDivisions.push_back( string("16") );		// default # of divisions is 16

	// get the sweep angle
	vector<string> sweepAngles = BZWParser::getValuesByKey("angle", header, coneData);
	if( sweepAngles.size() > 1 ) {
		printf("cone::update(): error! defined \"divisions\" %d times\n", (int)sweepAngles.size() );
		return 0;
	}
	if( sweepAngles.size() == 0 )
		sweepAngles.push_back( string("360") );		// default sweep is 360

	// get flatshading
	vector<string> flatShadings = BZWParser::getValuesByKey("flatshading", header, coneData);

	// get smoothbounce
	vector<string> smoothBounces =  BZWParser::getValuesByKey("smoothbounce", header, coneData);

	// do base class update
	if(!bz2object::update(data))
		return 0;


	// see if the divisions changed (if so, then update the geometry)
	int oldDivisions = divisions;
	if (vDivisions.size() > 0) 
		divisions = atoi( vDivisions[0].c_str() );
	

	float oldSweepAngle = sweepAngle;
	if (sweepAngles.size() > 0)
		sweepAngle = atof( sweepAngles[0].c_str() );

	// if the number of divisions changed or the sweep angle changed, rebuild the geometry
	if( divisions != oldDivisions || sweepAngle != oldSweepAngle ) {
		buildGeometry();
	}

	if (texsizes.size() > 0) {
		texsize = Point2D( texsizes[0].c_str() );
	}

	flatShading = (flatShadings.size() == 0 ? false : true);
	updateShadeModel();		// update the shade model

	smoothbounce = (smoothBounces.size() == 0 ? false : true);

	return 1;
}

// event handler
int cone::update( UpdateMessage& message ) {

	// superclass event handler
	int result = bz2object::update( message );

	switch( message.type ) {
		case UpdateMessage::SET_POSITION: 	// handle a new position
			setPos( *(message.getAsPosition()) );
			break;

		case UpdateMessage::SET_POSITION_FACTOR:	// handle a translation
			setPos( getPos() + *(message.getAsPositionFactor()) );
			break;

		case UpdateMessage::SET_ROTATION:		// handle a new rotation
			setRotation( *(message.getAsRotation()) );
			break;

		case UpdateMessage::SET_ROTATION_FACTOR:	// handle an angular translation
			setRotation( getRotation() + *(message.getAsRotationFactor()) );
			break;

		case UpdateMessage::SET_SCALE:		// handle a new scale
			setSize( *(message.getAsScale()) );
			break;

		case UpdateMessage::SET_SCALE_FACTOR:	// handle a scaling factor
			setSize( getSize() + *(message.getAsScaleFactor()) );
			break;

		default:	// unknown event; don't handle
			return result;
	}

	return 1;
}


// toString
string cone::toString(void) {
    string ret;
	if (!pyramidStyle)
		ret += "cone\n";
	else
		ret += "meshpyr\n";

	ret += BZWLines( this );

	// materials
	for (int i = 0; i < MaterialCount; i++) {
		osg::Group* group = (osg::Group*)getThisNode();
		osg::StateSet* stateset = group->getChild(i)->getStateSet();
		if (stateset->getName() != "")
			ret += "  " + string(sideNames[i]) + " matref " + stateset->getName();
	}

	// some options shouldn't be included for a meshpyr
	if (!pyramidStyle)
		ret += "  divisions " + string(itoa(divisions)) + "\n" +
			   "  angle " + string(ftoa(sweepAngle) ) + "\n";

	ret += string("") +
		   (flatShading == true ? "  flatshading\n" : "") +
		   (smoothbounce == true ? "  smoothbounce\n" : "") +
		   "  texsize " + texsize.toString() + "\n" +
		   (flipz == true ? "  flipz\n" : "") +
		   "end\n";

	return ret;
}

void cone::setFlatShading(bool value) {
	flatShading = value;
	updateShadeModel();
}

void cone::setSweepAngle(float value) {
	if( value != sweepAngle ) {		// refresh the geometry
		buildGeometry();
	}
	
	sweepAngle = value;
}
	
void cone::setDivisions(int value) {
	if( value != divisions ) {	// refresh the geometry
		buildGeometry();
	}
		
	divisions = value;
}

// build the cone geometry
void cone::buildGeometry() {
	// make the group
	osg::Group* theCone = (osg::Group*)getThisNode();
	osg::Geode* coneNode = (osg::Geode*)theCone->getChild( 0 );
	if (coneNode->getNumDrawables() > 0) coneNode->removeDrawables( 0 );
	osg::Geode* baseNode = (osg::Geode*)theCone->getChild( 1 );
	if (baseNode->getNumDrawables() > 0) baseNode->removeDrawables( 0 );
	osg::Geode* startNode = (osg::Geode*)theCone->getChild( 2 );
	if (startNode->getNumDrawables() > 0) startNode->removeDrawables( 0 );
	osg::Geode* endNode = (osg::Geode*)theCone->getChild( 3 );
	if (endNode->getNumDrawables() > 0) endNode->removeDrawables( 0 );

	// geometry data for the conical component
	osg::Vec3Array* points = new osg::Vec3Array();
	osg::DrawElementsUInt* indices = new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLE_FAN, 0 );
	osg::Vec2Array* texCoords = new osg::Vec2Array();

	// geometry for the base of the cone
	osg::DrawElementsUInt* baseIndices = new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLE_STRIP, 0 );
	osg::Vec2Array* baseTexCoords = new osg::Vec2Array();

   	// add the tip of the cone to the conical geometry
   	points->push_back( osg::Vec3( 0.0, 0.0, 1.0 ) );	// the tip of the cone
   	indices->push_back( 0 );	// the index of the tip of the cone
   	texCoords->push_back( osg::Vec2( 0.5, 0.5 ) );	// texture coordinate of the tip of the cone
   	baseTexCoords->push_back( osg::Vec2( 0.5, 0.5) );	// just a space-holder here

   	// build the base
   	float radius_x = 1.0;
   	float radius_y = 1.0;
   	float angle;

	// build the geodes
	osg::Geometry* coneGeometry = new osg::Geometry();
	osg::Geometry* baseGeometry = new osg::Geometry();
	coneNode->addDrawable( coneGeometry );
	coneGeometry->setVertexArray( points );
	coneGeometry->setTexCoordArray( 0, texCoords );
	coneGeometry->addPrimitiveSet( indices );
	baseNode->addDrawable( baseGeometry );
	baseGeometry->setVertexArray( points );
	baseGeometry->setTexCoordArray( 0, baseTexCoords );
	baseGeometry->addPrimitiveSet( baseIndices );

   	// build a full cone if the sweep angle is >= 360.0 degrees
   	if( sweepAngle >= 360.0f ) {
	   	for( int i = 0; i < divisions; i++ ) {
	   		angle = 2.0 * osg::PI * ((float)i / (float)divisions);

	   		// add the vertex
	   		points->push_back( osg::Vec3( cos(angle) * radius_x, sin(angle) * radius_y, 0 ) );

	   		// add the index of that vertex to the conical geometry
	   		indices->push_back( i+1 );

	   		// add the texture coordinate of that vertex to the concial geometry
	   		texCoords->push_back( osg::Vec2( 0.5 + 0.5 * cos(angle), 0.5 + 0.5 * sin(angle) ) );

	   		// add the texture coordinate of that vertex to the base geometry
	   		baseTexCoords->push_back( osg::Vec2( 0.5 + 0.5 * cos(angle), 0.5 + 0.5 * sin(angle) ) );
	   	}

	   	// add the final face to connect the last index to the first
	   	indices->push_back( 1 );

	   	// build the base indices
	   	baseIndices->push_back(1);
	   	baseIndices->push_back(2);
	   	for( int i = 3; i <= (divisions >> 1) + 1; i++ ) {
	   		baseIndices->push_back( divisions - i + 3 );
	   		baseIndices->push_back( i );
	   	}
	   	// if we have an odd # of divisions, add the final face
	   	if( divisions % 2 == 1 ) {
	   		baseIndices->push_back( (divisions>>1) + 2 );
	   	}
   	}
   	else {			// build a section of a cone
   		// add the center of the cone
   		points->push_back( osg::Vec3( 0, 0, 0 ) );

   		// it's texture coordinate
   		baseTexCoords->push_back( osg::Vec2( 0.5, 0.5 ) );

   		// place-holder texture coordinate
   		texCoords->push_back( osg::Vec2( 0.5, 0.5 ) );

   		float sweepAngleRads = sweepAngle * osg::PI / 180.0f;
   		for( int i = 0; i <= divisions; i++ ) {
	   		angle = sweepAngleRads * ((float)i / (float)divisions);

	   		// add the vertex
	   		points->push_back( osg::Vec3( cos(angle) * radius_x, sin(angle) * radius_y, 0 ) );

	   		// add the index of that vertex to the conical geometry
	   		indices->push_back( i+2 );

	   		// add the texture coordinate of that vertex to the concial geometry
	   		texCoords->push_back( osg::Vec2( 0.5 + 0.5 * cos(angle), 0.5 + 0.5 * sin(angle) ) );

	   		// add the texture coordinate of that vertex to the base geometry
	   		baseTexCoords->push_back( osg::Vec2( 0.5 + 0.5 * cos(angle), 0.5 + 0.5 * sin(angle) ) );
	   	}


	   	// build the base indices
	   	// switch to a triangle fan
	   	baseIndices->setMode( osg::PrimitiveSet::TRIANGLE_FAN );

	   	// make the indices
	   	// the first point should index the center of the cone
	   	baseIndices->push_back( 1 );	// the 1st point is the center
	   	for( int i = 2; i <= divisions + 2; i++ ) {
	   		baseIndices->push_back( i );
	   	}

	   	// make the cross-sections
	   	osg::DrawElementsUInt* startIndices = new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLES, 0 );
		osg::DrawElementsUInt* endIndices = new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLES, 0 );


	   	// add the indices to (1) the last point in the base, (2) the center, (3) the tip, and (4) the first point in the base
	   	endIndices->push_back( divisions + 2 );	// the (divisions+2)-th point is the last point in the base
	   	endIndices->push_back( 1 );	// the 1st point is the center
	   	endIndices->push_back( 0 );			// the 0th point is the tip

		startIndices->push_back( 2 );			// the 2st point is the first point in the base
		startIndices->push_back( 0 );			// the 0th point is the tip
		startIndices->push_back( 1 );	// the 1st point is the center
	   	
	   	// make the cross-section texture coordinates
	   	osg::Vec2Array* crossSectionTexCoords = new osg::Vec2Array();

	   	// make sure we correspond to the indices
	   	crossSectionTexCoords->push_back( osg::Vec2( 0.0, 0.0 ) );
	   	crossSectionTexCoords->push_back( osg::Vec2( 0.0, 0.5 ) );
	   	crossSectionTexCoords->push_back( osg::Vec2( 1.0, 0.5 ) );
	   	// insert place-holders
	   	for( int i = 0; i <= divisions + 3; i++ )
	   		crossSectionTexCoords->push_back( osg::Vec2( 1.0, 0.0 ) );

	   	crossSectionTexCoords->push_back( osg::Vec2( 0.0, 0.0 ) );

	   	// make the crossSection geode
	   	osg::Geometry* startGeometry = new osg::Geometry();
		osg::Geometry* endGeometry = new osg::Geometry();
		startNode->addDrawable( startGeometry );
		startGeometry->setVertexArray( points );
		startGeometry->setTexCoordArray( 0, crossSectionTexCoords );
		startGeometry->addPrimitiveSet( startIndices );
		endNode->addDrawable( endGeometry );
		endGeometry->setVertexArray( points );
		endGeometry->setTexCoordArray( 0, crossSectionTexCoords );
		endGeometry->addPrimitiveSet( endIndices );
   	}
}

// set the shade model based on the value of flatShading
void cone::updateShadeModel() {
	// get state set
	osg::StateSet* states = getOrCreateStateSet();

	// get the shade model
	osg::ShadeModel* shadeModel = dynamic_cast< osg::ShadeModel* >( states->getAttribute( osg::StateAttribute::SHADEMODEL ) );
	if( shadeModel == NULL ) {
		shadeModel = new osg::ShadeModel();		// if one doesn't exist, then make one
	}

	if( flatShading ) {
		shadeModel->setMode( osg::ShadeModel::FLAT );
	}
	else {
		shadeModel->setMode( osg::ShadeModel::SMOOTH );
	}

	// set the shade model
	states->setAttribute( shadeModel );
}
