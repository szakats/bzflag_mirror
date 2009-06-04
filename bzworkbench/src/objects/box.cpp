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

#include "objects/box.h"

const char* box::faceNames[FaceCount] = {
  "x+",
  "x-",
  "y+",
  "y-",
  "z+",
  "z-"
};

// constructors
box::box() : bz2object("box", "<name><position><rotation><size><matref><phydrv><drivethrough><shootthrough><passable><ricochet><texsize><texoffset>") {
	setDefaults();
}

box::box(string& data) : bz2object("box", "<name><position><rotation><size><matref><phydrv><drivethrough><shootthrough><passable><ricochet><texsize><texoffset>") {
	setDefaults();

	this->update( data );
}

// nothing to destroy...
box::~box() { }

void box::setDefaults() {
	updateGeometry();

	osg::Group* group = (osg::Group*)getThisNode();

	for (int i = 0; i < FaceCount; i++) {
		const float defScale = (i >= ZP) ? -2.0f : -8.0f;
		texSizes[i] = Point2D(defScale, defScale);
		texOffsets[i] = Point2D(0.0f, 0.0f);
		physDrvs[i] = NULL;
		driveThroughs[i] = false;
		shootThroughs[i] = false;
		ricochets[i] = false;

		// set default textures
		SceneBuilder::assignTexture((i >= ZP) ? "roof" : "boxwall", group->getChild(i),
				osg::StateAttribute::ON);
	}

	setPos( osg::Vec3(0.0, 0.0, 0.0) );
	setSize( osg::Vec3( 10.0, 10.0, 10.0 ) );

	SceneBuilder::markUnselected( this );
}

// getter
string box::get(void) {
	return toString();
}

// setter (string data)
int box::update(string& data) {
	// get the header
	const char* header = getHeader().c_str();

	// get the sections
	vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());

	// quit if there aren't any
	if(lines[0] == BZW_NOT_FOUND)
		return 0;

	if(!hasOnlyOne(lines, "box"))
		return 0;

	const char* boxData = lines[0].c_str();

	for (int i = 0; i < FaceCount; i++) {
		string faceName(faceNames[i]);
		vector<string> faces;
		faces.push_back(faceNames[i]);

		// need to be able to parse the meshbox face names
		if (i <= YN) {
			faces.push_back("sides");
			faces.push_back("outside");
		}
		else if (i == ZP) {
			faces.push_back("top");
		}
		else if (i == ZN) {
			faces.push_back("bottom");
		}


		vector<string> physdrvs = BZWParser::getValuesByKeyAndFaces("phydrv", faces, header, boxData);
		if(physdrvs.size() > 1) {
			printf("box::update(): Error! Defined \"%s phydrv\" %d times!\n", faceName.c_str(), (int)physdrvs.size());
			return 0;
		}

		vector<string> texsizes = BZWParser::getValuesByKeyAndFaces("texsize", faces, header, boxData);
		if(texsizes.size() > 1) {
			printf("box::update(): Error! Defined \"%s texsize\" %d times!\n", faceName.c_str(), (int)texsizes.size());
			return 0;
		}

		vector<string> texoffsets = BZWParser::getValuesByKeyAndFaces("texoffset", faces, header, boxData);
		if(texoffsets.size() > 1) {
			printf("box::update(): Error! Defined \"%s texoffset\" %d times!\n", faceName.c_str(), (int)texoffsets.size());
			return 0;
		}

		vector<string> drivethroughs = BZWParser::getValuesByKeyAndFaces("drivethrough", faces, header, boxData);
		if(drivethroughs.size() > 1) {
			printf("box::update(): Error! Defined \"%s drivethrough\" %d times!\n", faceName.c_str(), (int)drivethroughs.size());
			return 0;
		}

		vector<string> passables = BZWParser::getValuesByKeyAndFaces("passable", faces, header, boxData);
		if(passables.size() > 1) {
			printf("box::update(): Error! Defined \"%s passable\" %d times!\n", faceName.c_str(), (int)passables.size());
			return 0;
		}

		vector<string> shootthroughs = BZWParser::getValuesByKeyAndFaces("shootthrough", faces, header, boxData);
		if(shootthroughs.size() > 1) {
			printf("box::update(): Error! Defined \"%s shootthrough\" %d times!\n", faceName.c_str(), (int)shootthroughs.size());
			return 0;
		}

		vector<string> ricochets = BZWParser::getValuesByKeyAndFaces("drivethrough", faces, header, boxData);
		if(ricochets.size() > 1) {
			printf("box::update(): Error! Defined \"%s ricochet\" %d times!\n", faceName.c_str(), (int)ricochets.size());
			return 0;
		}

		vector<string> matrefs = BZWParser::getValuesByKeyAndFaces("matref", faces, header, boxData);

		if (physdrvs.size() > 0) {
			physics* phys = (physics*)Model::command( MODEL_GET, "phydrv", physdrvs[0] );
			if (phys != NULL)
				physDrvs[i] = phys;
			else
				printf("box::update(): Error! Couldn't find physics driver %s\n", physdrvs[0].c_str());
		}

		if (texsizes.size() > 0) {
			texSizes[i] = Point2D( texsizes[0].c_str() );
		}

		if (texoffsets.size() > 0) {
			texSizes[i] = Point2D( texoffsets[0].c_str() );
		}

		if (passables.size() > 0) {
			driveThroughs[i] = true;
			shootThroughs[i] = true;
		}

		if (drivethroughs.size() > 0) {
			driveThroughs[i] = true;
		}

		if (shootthroughs.size() > 0) {
			shootThroughs[i] = true;
		}

		if (ricochets.size() > 0) {
			this->ricochets[i] = true;
		}

		vector< material* > materials;
		for (vector<string>::iterator itr = matrefs.begin(); itr != matrefs.end(); itr++) {
			material* mat = (material*)Model::command( MODEL_GET, "material", *itr );
			if (mat != NULL)
				materials.push_back( mat );
			else
				printf("box::update(): Error! Couldn't find material %s\n", (*itr).c_str());
		}

		material* finalMat = material::computeFinalMaterial(materials);

		osg::Group* group = (osg::Group*)getThisNode();
		group->getChild(i)->setStateSet(finalMat);
	}


	osg::Vec3 size = getSize();

	int result = bz2object::update( data );
	if( result == 0 )
		return result;

	// if size changes then UVs must be regenerated
	if( getSize() != size ) {
		setSize(getSize());
	}

	return result;
}

// setter (with binary data)
// NOTE: don't call superclass update method, because it deals only with transformations (which are n/a here)
int box::update(UpdateMessage& message) {

	int result = bz2object::update( message );

	switch( message.type ) {
		case UpdateMessage::SET_POSITION: 	// handle a new position
			setPos( *(message.getAsPosition()) );
			break;

		case UpdateMessage::SET_POSITION_FACTOR:	// handle a translation
			setPos( getPos() + *(message.getAsPositionFactor()) );
			break;

		case UpdateMessage::SET_ROTATION:		// handle a new rotation
			setRotationZ( message.getAsRotation()->z() );
			break;

		case UpdateMessage::SET_ROTATION_FACTOR:	// handle an angular translation
			setRotationZ( getRotation().z() + message.getAsRotationFactor()->z() );
			break;

		case UpdateMessage::SET_SCALE: {	// handle a new scale (only scale one axis at a time)
			setSize( *(message.getAsScale()) );
			break;
		}

		case UpdateMessage::SET_SCALE_FACTOR: {	// handle a scaling factor (again, 1 axis at a time)
			setSize( getSize() + *(message.getAsScaleFactor()) );
			break;
		}
		default:	// unknown event; don't handle
			return result;
	}

	return 1;
}

// toString
string box::toString(void) {
	string faceLines;

	for (int i = 0; i < FaceCount; i++) {
		if (physDrvs[i] != NULL)
			faceLines += string(faceNames[i]) + " phydrv " + physDrvs[i]->getName() + "\n";
		faceLines += string(faceNames[i]) + " texsize " + texSizes[i].toString() + "\n";
		faceLines += string(faceNames[i]) + " texoffset " + texOffsets[i].toString() + "\n";
		if (driveThroughs[i])
			faceLines += string(faceNames[i]) + " drivethrough\n";
		if (shootThroughs[i])
			faceLines += string(faceNames[i]) + " shootthrough\n";
		if (ricochets[i])
			faceLines += string(faceNames[i]) + " ricochet\n";
	}

	return string("box\n") +
				  BZWLines( this ) +
				  faceLines +
				  "end\n";
}

void box::setSize( osg::Vec3 newSize ) {
	Primitives::rebuildBoxUV((osg::Group*)getThisNode(), newSize);
	bz2object::setSize( newSize );
}

void box::updateGeometry() {
	// make 1 by 1 by 1 box which can be scaled to the proper size
	osg::Group* group = Primitives::buildUntexturedBox( osg::Vec3( 1, 1, 1 ) );

	// make UV coordinates
	Primitives::rebuildBoxUV( group, getSize() );

	setThisNode( group );
}
