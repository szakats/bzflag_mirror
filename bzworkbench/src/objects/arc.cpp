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

#include "objects/arc.h"

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

const char* arc::sideNames[MaterialCount] = {
  "top",
  "bottom",
  "inside",
  "outside",
  "startside",
  "endside"
};

// default constructor
arc::arc() :
	bz2object("arc", "<position><rotation><size><flatshading><angle><ratio><name><divisions><shift><shear><spin><scale><smoothbounce><phydrv><matref>") {
	setDefaults();
}

// data constructor
arc::arc(string& data) :
	bz2object("arc", "<position><rotation><size><flatshading><angle><ratio><name><divisions><shift><shear><spin><scale><smoothbounce><phydrv><matref>", data.c_str()) {
	setDefaults();

	update(data);
}

void arc::setDefaults() {
	// define some basic values
	ratio = 1.0f;
	divisions = 16;
	angle = 360.0f;
	physicsDriver = NULL;
	flatShading = false;
	smoothbounce = false;
	texsize.set( -8.0f, -8.0f, -8.0f, -8.0f  );
	boxStyle = false;

	osg::Group* group = new osg::Group();
	for (int i = 0; i < MaterialCount; i++)
		group->addChild( new osg::Geode() );

	SceneBuilder::assignTexture( "roof", group->getChild( 0 ), osg::StateAttribute::ON);
	SceneBuilder::assignTexture( "roof", group->getChild( 1 ), osg::StateAttribute::ON);
	SceneBuilder::assignTexture( "boxwall", group->getChild( 2 ), osg::StateAttribute::ON);
	SceneBuilder::assignTexture( "boxwall", group->getChild( 3 ), osg::StateAttribute::ON);
	SceneBuilder::assignTexture( "wall", group->getChild( 4 ), osg::StateAttribute::ON);
	SceneBuilder::assignTexture( "wall", group->getChild( 5 ), osg::StateAttribute::ON);

	setThisNode( group );

	setSize( osg::Vec3( 10, 10, 10 ) );

}

// getter
string arc::get(void) { return toString(); }

// setter
int arc::update(string& data) {
	const char* header = getHeader().c_str();
	// get the chunk we need

	vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());

	// check and see if the proper data segment was found
	if(lines[0] == BZW_NOT_FOUND)
		return 0;

	string key = (boxStyle == true ? "meshbox" : "arc");
	if(!hasOnlyOne(lines, key.c_str()))
		return 0;

	const char* arcData = lines[0].c_str();

	// get the matrefs
	osg::Group* group = (osg::Group*)getThisNode();
	for (int i = 0; i < MaterialCount; i++) {
		vector<string> faces;
		faces.push_back(sideNames[i]);

		vector<string> matrefs = BZWParser::getValuesByKeyAndFaces("matref", faces, header, arcData);

		if (matrefs.size() > 0) {
			vector< material* > materials;
			for (vector<string>::iterator itr = matrefs.begin(); itr != matrefs.end(); itr++) {
				material* mat = (material*)Model::command( MODEL_GET, "material", *itr );
				if (mat != NULL)
					materials.push_back( mat );
				else
					printf("arc::update(): Error! Couldn't find material %s\n", (*itr).c_str());
			}

			material* finalMat = material::computeFinalMaterial(materials);

			group->getChild(i)->setStateSet(finalMat);
		}
	}

	// get the texsize
	vector<string> texsizes = BZWParser::getValuesByKey("texsize", header, arcData);
	if(texsizes.size() > 1) {
		printf("arc::update(): Error! Defined \"texsize\" %d times!\n", (int)texsizes.size());
		return 0;
	}

	// get the angle
	vector<string> angles = BZWParser::getValuesByKey("angle", header, arcData);
	if(!hasOnlyOne(angles, "angle"))
		return 0;

	// get the divisions
	vector<string> vDivisions = BZWParser::getValuesByKey("divisions", header, arcData);
	if(!hasOnlyOne(vDivisions, "divisions"))
		return 0;

	// get the ratio
	vector<string> ratios = BZWParser::getValuesByKey("ratio", header, arcData);
	if(!hasOnlyOne(ratios, "ratio"))
		return 0;

	// get flatshading
	vector<string> flatShadings = BZWParser::getValuesByKey("flatshading", header, arcData);

	// get smoothbounce
	vector<string> smoothBounces =  BZWParser::getValuesByKey("smoothbounce", header, arcData);
	
	// do base class update
	if(!bz2object::update(data))
		return 0;

	// set the data
	if ( angles.size() > 0 )
		angle = atof( angles[0].c_str() );
	if ( vDivisions.size() > 0 )
		divisions = atoi( vDivisions[0].c_str() );
	if ( ratios.size() > 0 )
		ratio = atof( ratios[0].c_str() );
	if ( texsizes.size() > 0 ) {
		texsize = Point4D( texsizes[0].c_str() );
	}
	flatShading = (flatShadings.size() == 0 ? false : true);
	smoothbounce = (smoothBounces.size() == 0 ? false : true);

	updateGeometry();

	return 1;
}

int arc::update(UpdateMessage& message) {
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

		case UpdateMessage::SET_SCALE:		// handle a new scale
			setSize( *(message.getAsScale()) );
			break;

		case UpdateMessage::SET_SCALE_FACTOR:	// handle a scaling factor
			setSize( getSize() + *(message.getAsScaleFactor()) );
			break;

		default:	// unknown event; don't handle
			return 0;
	}

	return 1;
}

// toString
string arc::toString(void) {
	string ret;
	if (!boxStyle)
		ret += "arc\n";
	else
		ret += "meshbox\n";

	ret += BZWLines( this );

	// materials
	for (int i = 0; i < MaterialCount; i++) {
		osg::Group* group = (osg::Group*)getThisNode();
		osg::StateSet* stateset = group->getChild(i)->getStateSet();
		if (stateset->getName() != "")
			ret += "  " + string(sideNames[i]) + " matref " + stateset->getName();
	}

	// some options shouldn't be included for a meshbox
	if (!boxStyle)
		ret += "  divisions " + string(itoa(divisions)) + "\n" +
			   "  angle " + string(ftoa(angle) ) + "\n" +
			   "  ratio " + string(ftoa(ratio)) + "\n" +
			   (flatShading == true ? "  flatshading\n" : "");

	ret += string("") +
		   (smoothbounce == true ? "  smoothbounce\n" : "") +
		   "  texsize " + texsize.toString() + "\n" +
		   "end\n";

	return ret;
}

// render
int arc::render(void) {
	return 0;
}

void arc::setSize( osg::Vec3 newSize ) {
	realSize = newSize;
	updateGeometry();
}

osg::Vec3 arc::getSize() {
	return realSize;
}

void arc::updateGeometry() {
	osg::Group* arc = (osg::Group*)getThisNode();

	// clear any previous geometry and make new geometry
	osg::Geometry* sides[MaterialCount];
	for (int i = 0; i < MaterialCount; i++) {
		osg::Geode* geode = (osg::Geode*)arc->getChild( i );
		if ( geode->getNumDrawables() > 0 )
			geode->removeDrawables( 0 );
		sides[i] = new osg::Geometry();
		geode->addDrawable( sides[i] );
	}

	bool isPie = false;    // has no inside edge
	bool isCircle = false; // angle of 360 degrees
	const float minSize = 1.0e-6f; // cheezy / lazy

	// set size
	osg::Vec3 sz = realSize;

	// validity checking
	if ((sz.x() < minSize) || (sz.y() < minSize) || (sz.z() < minSize) ||
		(fabsf(texsize.x()) < minSize) || (fabsf(texsize.y()) < minSize) ||
		(fabsf(texsize.z()) < minSize) || (fabsf(texsize.w()) < minSize) ||
		(ratio < 0.0f) || (ratio > 1.0f)) {
			return;
	}

	// adjust the texture sizes   FIXME: finish texsz[2] & texsz[3]
	osg::Vec4 texsz(texsize.x(), texsize.y(), texsize.z(), texsize.w());
	if (texsz.x() < 0.0f) {
		// unless you want to do elliptic integrals, here's
		// the Ramanujan approximation for the circumference
		// of an ellipse  (it will be rounded anyways)
		const float circ =
			(float)M_PI * ((3.0f * (sz.x() + sz.y())) -
			sqrtf((sz.x() + (3.0f * sz.y())) * (sz.y() + (3.0f * sz.x()))));
		// make sure it's an integral number so that the edges line up
		texsz._v[0] = -floorf(circ / texsz.x());
	}
	if (texsz.y() < 0.0f) {
		texsz._v[1] = -(sz.z() / texsz.y());
	}

	// setup the angles
	float a = angle;
	if (a > +360.0f) {
		a = +360.0f;
	}
	if (a < -360.0f) {
		a = -360.0f;
	}
	a = a * (float)(M_PI / 180.0); // convert to radians
	if (a < 0.0f) {
		a = -a;
	}

	// more validity checking
	if (divisions <= (int) ((a + minSize) / M_PI)) {
		return;
	}

	if (fabsf ((float)M_PI - fmodf (a + (float)M_PI, (float)M_PI * 2.0f)) < minSize) {
		isCircle = true;
	}

	// setup the radii
	float inrad = sz.x() * (1.0f - ratio);
	float outrad = sz.x();
	if (inrad > outrad) {
		const float tmp = inrad;
		inrad = outrad;
		outrad = tmp;
	}
	if ((outrad < minSize) || ((outrad - inrad) < minSize)) {
		return;
	}
	if (inrad < minSize) {
		isPie = true;
	}
	const float squish = sz.y() / sz.x();

	if (isPie) {
		makePie(sides, isCircle, a, 0, sz.z(), outrad, squish, texsz);
	} else {
		makeRing(sides, isCircle, a, 0, sz.z(), inrad, outrad, squish, texsz);
	}

	
}


void arc::makePie(osg::Geometry** sides, bool isCircle, float a, float r,
				   float h, float radius, float squish,
				   osg::Vec4& texsz)
{
	int i;

	// setup the coordinates
	osg::Vec3Array* vertices = new osg::Vec3Array();
	osg::Vec2Array* texcoords = new osg::Vec2Array();
	osg::Vec3 v;
	osg::Vec2 t;

	// setup the texsize across the disc
	if (texsz.z() < 0.0f) {
		texsz._v[2] = -((2.0f * radius) / texsz.z());
	}
	if (texsz.w() < 0.0f) {
		texsz._v[3] = -((2.0f * radius * squish) / texsz.w());
	}

	const float astep = a / (float) divisions;

	for (i = 0; i < (divisions + 1); i++) {
		float ang = r + (astep * (float)i);
		float cos_val = cosf(ang);
		float sin_val = sinf(ang);

		// vertices and normals
		if (!isCircle || (i != divisions)) {
			osg::Vec2 delta;
			delta.set( cos_val * radius,
					   (sin_val * radius) * squish );
			// vertices
			v.set( delta.x(),
				   delta.y(),
				   0);
			vertices->push_back( v );
			v._v[2] = v.z() + h;
			vertices->push_back( v );
		}

		// texture coordinates (around the edge)
		t.set( (float) i / (float) divisions * texsz.x(), 0.0f );
		texcoords->push_back(t);
		// outside texcoord
		t._v[1] = texsz.y() * 1.0f;
		texcoords->push_back(t);
	}

	// texture coordinates (around the disc)
	for (i = 0; i < (divisions + 1); i++) {
		float ang = astep * (float)i;
		float cos_val = cosf(ang);
		float sin_val = sinf(ang);
		t.set( texsz.z() * (0.5f + (0.5f * cos_val)),
			   texsz.w() * (0.5f + (0.5f * sin_val)) );
		texcoords->push_back(t);
	}

	// the central coordinates
	v.set( 0, 0, 0 );
	vertices->push_back(v); // bottom
	v._v[2] = h;
	vertices->push_back(v); // top
	t.set( texsz.z() * 0.5f, texsz.w() * 0.5f);
	texcoords->push_back(t);

	// setup the face count
	int fcount = (divisions * 3);
	if (!isCircle) {
		fcount = fcount + 2; // add the start and end faces
	}

	// now make the faces
	int vlen, nlen;
	if (isCircle) {
		vlen = divisions * 2;
		nlen = divisions;
	} else {
		vlen = (divisions + 1) * 2;
		nlen = (divisions + 1);
	}

	const int vtop = vlen + 1;
	const int vbot = vlen;
	const int tmid = ((divisions + 1) * 3);

	osg::Vec3Array* realVertices[MaterialCount];
	osg::Vec2Array* realTexcoords[MaterialCount];

	for ( int j = 0; j < MaterialCount; j++ ) {
		realVertices[j] = new osg::Vec3Array();
		realTexcoords[j] = new osg::Vec2Array();
		sides[j]->setVertexArray( realVertices[j] );
		sides[j]->setTexCoordArray( 0, realTexcoords[j] );
	}
	osg::DrawElementsUInt* topIdx = new osg::DrawElementsUInt( osg::DrawArrays::TRIANGLES, 0 );
	osg::DrawElementsUInt* bottomIdx = new osg::DrawElementsUInt( osg::DrawArrays::TRIANGLES, 0 );
	osg::DrawElementsUInt* outsideIdx = new osg::DrawElementsUInt( osg::DrawArrays::QUADS, 0 );
	osg::DrawElementsUInt* startIdx = new osg::DrawElementsUInt( osg::DrawElements::QUADS, 0 );
	osg::DrawElementsUInt* endIdx = new osg::DrawElementsUInt( osg::DrawElements::QUADS, 0 );
	sides[0]->addPrimitiveSet( topIdx );
	sides[1]->addPrimitiveSet( bottomIdx );
	sides[3]->addPrimitiveSet( outsideIdx );
	sides[4]->addPrimitiveSet( startIdx );
	sides[5]->addPrimitiveSet( endIdx );
	
	int arrayPlace = 0;

	for (i = 0; i < divisions; i++) {

		// handy macros
#define PV(x) (((x) + (i * 2)) % vlen)
#define PTO(x) ((x) + (i * 2))		     // outside edge
#define PTC(x) (((divisions + 1) * 2) + (x) + i)   // around the disc
#define PTCI(x) (((divisions + 1) * 3) - (x) - i - 1)

		// outside
		realVertices[3]->push_back( (*vertices)[PV(0)] );
		realVertices[3]->push_back( (*vertices)[PV(2)] );
		realVertices[3]->push_back( (*vertices)[PV(3)] );
		realVertices[3]->push_back( (*vertices)[PV(1)] );
		realTexcoords[3]->push_back( (*texcoords)[PTO(0)] );
		realTexcoords[3]->push_back( (*texcoords)[PTO(2)] );
		realTexcoords[3]->push_back( (*texcoords)[PTO(3)] );
		realTexcoords[3]->push_back( (*texcoords)[PTO(1)] );
		for (int j = 0; j < 4; j++)
			outsideIdx->push_back( i*4 + j );

		// top
		realVertices[0]->push_back( (*vertices)[vtop] );
		realVertices[0]->push_back( (*vertices)[PV(1)] );
		realVertices[0]->push_back( (*vertices)[PV(3)] );
		realTexcoords[0]->push_back( (*texcoords)[tmid] );
		realTexcoords[0]->push_back( (*texcoords)[PTC(0)] );
		realTexcoords[0]->push_back( (*texcoords)[PTC(1)] );
		for (int j = 0; j < 3; j++)
			topIdx->push_back( i*3 + j );

		// bottom
		realVertices[1]->push_back( (*vertices)[vbot] );
		realVertices[1]->push_back( (*vertices)[PV(2)] );
		realVertices[1]->push_back( (*vertices)[PV(0)] );
		realTexcoords[1]->push_back( (*texcoords)[tmid] );
		realTexcoords[1]->push_back( (*texcoords)[PTCI(1)] );
		realTexcoords[1]->push_back( (*texcoords)[PTCI(0)] );
		for (int j = 0; j < 3; j++)
			bottomIdx->push_back( i*3 + j );
	}


	if (!isCircle) {
		int tc = (divisions * 2);
		// start face
		realVertices[4]->push_back( (*vertices)[vbot] );
		realVertices[4]->push_back( (*vertices)[0] );
		realVertices[4]->push_back( (*vertices)[1] );
		realVertices[4]->push_back( (*vertices)[vtop] );
		realTexcoords[4]->push_back( (*texcoords)[0] );
		realTexcoords[4]->push_back( (*texcoords)[tc] );
		realTexcoords[4]->push_back( (*texcoords)[tc+1] );
		realTexcoords[4]->push_back( (*texcoords)[1] );
		for (int i = 0; i < 4; i++)
			startIdx->push_back( i );

		// end face
		int e = divisions * 2;
		realVertices[5]->push_back( (*vertices)[e] );
		realVertices[5]->push_back( (*vertices)[vbot] );
		realVertices[5]->push_back( (*vertices)[vtop] );
		realVertices[5]->push_back( (*vertices)[e+1] );
		realTexcoords[5]->push_back( (*texcoords)[0] );
		realTexcoords[5]->push_back( (*texcoords)[tc] );
		realTexcoords[5]->push_back( (*texcoords)[tc+1] );
		realTexcoords[5]->push_back( (*texcoords)[1] );
		for (int i = 0; i < 4; i++)
			endIdx->push_back( i );
	}
}


void arc::makeRing(osg::Geometry** sides, bool isCircle, float a, float r,
				    float h, float inrad, float outrad,
					float squish, osg::Vec4& texsz)
{
	// setup the coordinates
	osg::Vec3Array* vertices = new osg::Vec3Array();
	osg::Vec2Array* texcoords = new osg::Vec2Array();
	osg::Vec3 v;
	osg::Vec2 t;

	int i;
	const float astep = a / (float) divisions;

	for (i = 0; i < (divisions + 1); i++) {
		float ang = r + (astep * (float)i);
		float cos_val = cosf(ang);
		float sin_val = sinf(ang);

		// vertices and normals
		if (!isCircle || (i != divisions)) {
			// inside points
			v.set( (cos_val * inrad),
				(squish * (sin_val * inrad)),
				0);
			vertices->push_back(v);
			v._v[2] = v.z() + h;
			vertices->push_back(v);
			// outside points
			v.set( (cos_val * outrad),
				   (squish * (sin_val * outrad)),
				   0);
			vertices->push_back(v);
			v._v[2] = v.z() + h;
			vertices->push_back(v);
		}

		// texture coordinates
		// inside texcoord
		t.set( (float) i / (float) divisions * texsz.x(), 0.0f);
		texcoords->push_back(t);
		// outside texcoord
		t._v[1] = texsz.y() * 1.0f;
		texcoords->push_back(t);
	}

	// setup the face count
	int fcount = (divisions * 4);
	if (!isCircle) {
		fcount = fcount + 2; // add the start and end faces
	}

	// now make the faces
	int vlen, nlen;
	if (isCircle) {
		vlen = divisions * 4;
		nlen = divisions * 2;
	} else {
		vlen = (divisions + 1) * 4;
		nlen = (divisions + 1) * 2;
	}

	osg::Vec3Array* realVertices[MaterialCount];
	osg::Vec2Array* realTexcoords[MaterialCount];

	osg::DrawElementsUInt* indices[MaterialCount];

	for (int j = 0; j < MaterialCount; j++ ) {
		realVertices[j] = new osg::Vec3Array();
		realTexcoords[j] = new osg::Vec2Array();
		indices[j] = new osg::DrawElementsUInt( osg::DrawElements::QUADS, 0 );
		sides[j]->setVertexArray( realVertices[j] );
		sides[j]->setTexCoordArray( 0, realTexcoords[j] );
		sides[j]->addPrimitiveSet( indices[j] );
	}
	

	int arrayPlace = 0;

	for (i = 0; i < divisions; i++) {

		// handy macros
#define RV(x) (((x) + (i * 4)) % vlen)
#define RT(x) ((x) + (i * 2))
#define RIT(x) ((divisions + ((x)%2))*2 - ((x) + (i * 2)))

		// inside
		realVertices[2]->push_back( (*vertices)[RV(4)] );
		realVertices[2]->push_back( (*vertices)[RV(0)] );
		realVertices[2]->push_back( (*vertices)[RV(1)] );
		realVertices[2]->push_back( (*vertices)[RV(5)] );
		realTexcoords[2]->push_back( (*texcoords)[RIT(2)] );
		realTexcoords[2]->push_back( (*texcoords)[RIT(0)] );
		realTexcoords[2]->push_back( (*texcoords)[RIT(1)] );
		realTexcoords[2]->push_back( (*texcoords)[RIT(3)] );
		for (int j = 0; j < 4; j++)
			indices[2]->push_back( i*4 + j );

		// outside
		realVertices[3]->push_back( (*vertices)[RV(2)] );
		realVertices[3]->push_back( (*vertices)[RV(6)] );
		realVertices[3]->push_back( (*vertices)[RV(7)] );
		realVertices[3]->push_back( (*vertices)[RV(3)] );
		realTexcoords[3]->push_back( (*texcoords)[RT(0)] );
		realTexcoords[3]->push_back( (*texcoords)[RT(2)] );
		realTexcoords[3]->push_back( (*texcoords)[RT(3)] );
		realTexcoords[3]->push_back( (*texcoords)[RT(1)] );
		for (int j = 0; j < 4; j++)
			indices[3]->push_back( i*4 + j );

		// top
		realVertices[0]->push_back( (*vertices)[RV(3)] );
		realVertices[0]->push_back( (*vertices)[RV(7)] );
		realVertices[0]->push_back( (*vertices)[RV(5)] );
		realVertices[0]->push_back( (*vertices)[RV(1)] );
		realTexcoords[0]->push_back( (*texcoords)[RT(0)] );
		realTexcoords[0]->push_back( (*texcoords)[RT(2)] );
		realTexcoords[0]->push_back( (*texcoords)[RT(3)] );
		realTexcoords[0]->push_back( (*texcoords)[RT(1)] );
		for (int j = 0; j < 4; j++)
			indices[0]->push_back( i*4 + j );

		// bottom
		realVertices[1]->push_back( (*vertices)[RV(0)] );
		realVertices[1]->push_back( (*vertices)[RV(4)] );
		realVertices[1]->push_back( (*vertices)[RV(6)] );
		realVertices[1]->push_back( (*vertices)[RV(2)] );
		realTexcoords[1]->push_back( (*texcoords)[RT(0)] );
		realTexcoords[1]->push_back( (*texcoords)[RT(2)] );
		realTexcoords[1]->push_back( (*texcoords)[RT(3)] );
		realTexcoords[1]->push_back( (*texcoords)[RT(1)] );
		for (int j = 0; j < 4; j++)
			indices[1]->push_back( i*4 + j );
	}

	if (!isCircle) {
		int tc = (divisions * 2);
		// start face
		realVertices[4]->push_back( (*vertices)[0] );
		realVertices[4]->push_back( (*vertices)[2] );
		realVertices[4]->push_back( (*vertices)[3] );
		realVertices[4]->push_back( (*vertices)[1] );
		realTexcoords[4]->push_back( (*texcoords)[0] );
		realTexcoords[4]->push_back( (*texcoords)[tc] );
		realTexcoords[4]->push_back( (*texcoords)[tc+1] );
		realTexcoords[4]->push_back( (*texcoords)[1] );
		for (int j = 0; j < 4; j++)
			indices[4]->push_back( j );

		// end face
		int e = divisions * 4;
		realVertices[5]->push_back( (*vertices)[e+2] );
		realVertices[5]->push_back( (*vertices)[e] );
		realVertices[5]->push_back( (*vertices)[e+1] );
		realVertices[5]->push_back( (*vertices)[e+3] );
		realTexcoords[5]->push_back( (*texcoords)[0] );
		realTexcoords[5]->push_back( (*texcoords)[tc] );
		realTexcoords[5]->push_back( (*texcoords)[tc+1] );
		realTexcoords[5]->push_back( (*texcoords)[1] );
		for (int j = 0; j < 4; j++)
			indices[5]->push_back( j );
	}
}
