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

// default constructor
arc::arc() : 
	bz2object("arc", "<position><rotation><size><flatshading><angle><ratio><name><divisions><shift><shear><spin><scale><smoothbounce><phydrv><matref>") {
	
	// define some basic values
	ratio = 1.0f;
	divisions = 16;
	angle = 180.0f;
	setName("default_arc");
	physicsDriver = NULL;
	flatShading = false;
	smoothbounce = true;

	
	osg::Group* group = new osg::Group();
	group->addChild( new osg::Geode() );
	group->addChild( new osg::Geode() );

	setThisNode( group );

	setSize( osg::Vec3( 10, 10, 10 ) );
}

// data constructor
arc::arc(string& data) :
	bz2object("arc", "<position><rotation><size><flatshading><angle><ratio><name><divisions><shift><shear><spin><scale><smoothbounce><phydrv><matref>", data.c_str()) {
	
	// define some basic values
	ratio = 0.5f;
	divisions = 16;
	angle = 180.0f;
	setName("default_arc");
	physicsDriver = NULL;
	flatShading = false;
	smoothbounce = true;
	
	osg::Group* group = new osg::Group();
	group->addChild( new osg::Geode() );
	group->addChild( new osg::Geode() );

	setThisNode( group );

	setSize( osg::Vec3( 10, 10, 10 ) );

	update(data);
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
		
	if(!hasOnlyOne(lines, "arc"))
		return 0;
		
	const char* arcData = lines[0].c_str();
		
	// get the name
	vector<string> names = BZWParser::getValuesByKey("name", header, arcData);
	if(!hasOnlyOne(names, "name"))
		return 0;
		
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
	setName( names[0] );
	angle = atof( angles[0].c_str() );
	divisions = atoi( vDivisions[0].c_str() );
	ratio = atof( ratios[0].c_str() );
	flatShading = (flatShadings.size() == 0 ? false : true);
	smoothbounce = (smoothBounces.size() == 0 ? false : true);

	updateGeometry( osg::Vec4(1, 1, 1, 1) );
	
	return 1;	
}

// toString
string arc::toString(void) {
	return string("arc\n") +
				  BZWLines( this ) +
				  "  angle " + string(ftoa(angle)) + "\n" +
				  "  ratio " + string(ftoa(ratio)) + "\n" +
				  "  divisions " + string(itoa(divisions)) + "\n" +
				  (flatShading == true ? "  flatshading\n" : "") +
				  (smoothbounce == true ? "  smoothbounce\n" : "") + 
				  "end\n";
	
}

// render
int arc::render(void) {
	return 0;	
}

void arc::setSize( const osg::Vec3d& newSize) {
	bz2object::setSize( newSize );
	updateGeometry( osg::Vec4(1, 1, 1, 1) );
}

void arc::updateGeometry( const osg::Vec4& texsize ) {
	osg::Group* arc = (osg::Group*)getThisNode();
	osg::Geode* sides = (osg::Geode*)arc->getChild( 0 );
	osg::Geode* topBottom = (osg::Geode*)arc->getChild( 1 );
	
	// clear any previous geometry
	if ( sides->getNumDrawables() > 0 )
		sides->removeDrawables( 0 );
	if ( topBottom->getNumDrawables() > 0 )
		topBottom->removeDrawables( 0 );

	osg::Geometry* sideMesh = new osg::Geometry();
	osg::Geometry* topbotMesh = new osg::Geometry();
	sides->addDrawable( sideMesh );
	topBottom->addDrawable( topbotMesh );

	bool isPie = false;    // has no inside edge
	bool isCircle = false; // angle of 360 degrees
	const float minSize = 1.0e-6f; // cheezy / lazy
	
	// set size
	osg::Vec3 sz( 1, 1, 1 );

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
		makePie(sideMesh, topbotMesh, isCircle, a, 0, sz.z(), outrad, squish, texsz);
	} else {
		makeRing(sideMesh, topbotMesh, isCircle, a, 0, sz.z(), inrad, outrad, squish, texsz);
	}

	// load and apply textures
	osg::Texture2D* sideTexture = Primitives::loadTexture( "share/box/boxwall.png" );
	osg::Texture2D* topbotTexture = Primitives::loadTexture( "share/box/roof.png" );
	Primitives::setNodeTexture( sides, sideTexture );
	Primitives::setNodeTexture( topBottom, topbotTexture );
}


void arc::makePie(osg::Geometry* sideMesh, osg::Geometry* topbotMesh, bool isCircle, float a, float r,
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

	osg::Vec3Array* realVertices = new osg::Vec3Array();
	osg::Vec2Array* realTexcoords = new osg::Vec2Array();
	osg::DrawElementsUInt* quads = new osg::DrawElementsUInt( osg::DrawElements::QUADS, 0 );
	osg::DrawElementsUInt* tris = new osg::DrawElementsUInt( osg::DrawArrays::TRIANGLES, 0 );
	int arrayPlace = 0;

	for (i = 0; i < divisions; i++) {

		// handy macros
#define PV(x) (((x) + (i * 2)) % vlen)
#define PTO(x) ((x) + (i * 2))		     // outside edge
#define PTC(x) (((divisions + 1) * 2) + (x) + i)   // around the disc
#define PTCI(x) (((divisions + 1) * 3) - (x) - i - 1)

		// outside
		realVertices->push_back( (*vertices)[PV(0)] );
		realVertices->push_back( (*vertices)[PV(2)] );
		realVertices->push_back( (*vertices)[PV(3)] );
		realVertices->push_back( (*vertices)[PV(1)] );
		realTexcoords->push_back( (*texcoords)[PTO(0)] );
		realTexcoords->push_back( (*texcoords)[PTO(2)] );
		realTexcoords->push_back( (*texcoords)[PTO(3)] );
		realTexcoords->push_back( (*texcoords)[PTO(1)] );
		for (int j = 0; j < 4; j++)
			quads->push_back( arrayPlace++ );

		// top
		realVertices->push_back( (*vertices)[vtop] );
		realVertices->push_back( (*vertices)[PV(1)] );
		realVertices->push_back( (*vertices)[PV(3)] );
		realTexcoords->push_back( (*texcoords)[tmid] );
		realTexcoords->push_back( (*texcoords)[PTC(0)] );
		realTexcoords->push_back( (*texcoords)[PTC(1)] );
		for (int j = 0; j < 3; j++)
			tris->push_back( arrayPlace++ );

		// bottom
		realVertices->push_back( (*vertices)[vbot] );
		realVertices->push_back( (*vertices)[PV(2)] );
		realVertices->push_back( (*vertices)[PV(0)] );
		realTexcoords->push_back( (*texcoords)[tmid] );
		realTexcoords->push_back( (*texcoords)[PTCI(1)] );
		realTexcoords->push_back( (*texcoords)[PTCI(0)] );
		for (int j = 0; j < 3; j++)
			tris->push_back( arrayPlace++ );
	}


	if (!isCircle) {
		int tc = (divisions * 2);
		// start face
		realVertices->push_back( (*vertices)[vbot] );
		realVertices->push_back( (*vertices)[0] );
		realVertices->push_back( (*vertices)[1] );
		realVertices->push_back( (*vertices)[vtop] );
		realTexcoords->push_back( (*texcoords)[0] );
		realTexcoords->push_back( (*texcoords)[tc] );
		realTexcoords->push_back( (*texcoords)[tc+1] );
		realTexcoords->push_back( (*texcoords)[1] );
		for (int i = 0; i < 4; i++)
			quads->push_back( arrayPlace++ );

		// end face
		int e = divisions * 2;
		realVertices->push_back( (*vertices)[e] );
		realVertices->push_back( (*vertices)[vbot] );
		realVertices->push_back( (*vertices)[vtop] );
		realVertices->push_back( (*vertices)[e+1] );
		realTexcoords->push_back( (*texcoords)[0] );
		realTexcoords->push_back( (*texcoords)[tc] );
		realTexcoords->push_back( (*texcoords)[tc+1] );
		realTexcoords->push_back( (*texcoords)[1] );
		for (int i = 0; i < 4; i++)
			quads->push_back( arrayPlace++ );
	}

	sideMesh->setVertexArray( realVertices );
	sideMesh->setTexCoordArray( 0, realTexcoords );
	sideMesh->addPrimitiveSet( quads );
	topbotMesh->setVertexArray( realVertices );
	topbotMesh->setTexCoordArray( 0, realTexcoords );
	topbotMesh->addPrimitiveSet( tris );
}


void arc::makeRing(osg::Geometry* sideMesh, osg::Geometry* topbotMesh, bool isCircle, float a, float r,
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

	osg::Vec3Array* realVertices = new osg::Vec3Array();
	osg::Vec2Array* realTexcoords = new osg::Vec2Array();
	osg::DrawElementsUInt* quadsSide = new osg::DrawElementsUInt( osg::DrawElements::QUADS, 0 );
	osg::DrawElementsUInt* quadsTopBot = new osg::DrawElementsUInt( osg::DrawElements::QUADS, 0 );
	int arrayPlace = 0;

	for (i = 0; i < divisions; i++) {

		// handy macros
#define RV(x) (((x) + (i * 4)) % vlen)
#define RT(x) ((x) + (i * 2))
#define RIT(x) ((divisions + ((x)%2))*2 - ((x) + (i * 2)))

		// inside
		realVertices->push_back( (*vertices)[RV(4)] );
		realVertices->push_back( (*vertices)[RV(0)] );
		realVertices->push_back( (*vertices)[RV(1)] );
		realVertices->push_back( (*vertices)[RV(5)] );
		realTexcoords->push_back( (*texcoords)[RIT(2)] );
		realTexcoords->push_back( (*texcoords)[RIT(0)] );
		realTexcoords->push_back( (*texcoords)[RIT(1)] );
		realTexcoords->push_back( (*texcoords)[RIT(3)] );

		// outside
		realVertices->push_back( (*vertices)[RV(2)] );
		realVertices->push_back( (*vertices)[RV(6)] );
		realVertices->push_back( (*vertices)[RV(7)] );
		realVertices->push_back( (*vertices)[RV(3)] );
		realTexcoords->push_back( (*texcoords)[RT(0)] );
		realTexcoords->push_back( (*texcoords)[RT(2)] );
		realTexcoords->push_back( (*texcoords)[RT(3)] );
		realTexcoords->push_back( (*texcoords)[RT(1)] );
		for (int j = 0; j < 8; j++)
			quadsSide->push_back( arrayPlace++ );

		// top
		realVertices->push_back( (*vertices)[RV(3)] );
		realVertices->push_back( (*vertices)[RV(7)] );
		realVertices->push_back( (*vertices)[RV(5)] );
		realVertices->push_back( (*vertices)[RV(1)] );
		realTexcoords->push_back( (*texcoords)[RT(0)] );
		realTexcoords->push_back( (*texcoords)[RT(2)] );
		realTexcoords->push_back( (*texcoords)[RT(3)] );
		realTexcoords->push_back( (*texcoords)[RT(1)] );

		// bottom
		realVertices->push_back( (*vertices)[RV(0)] );
		realVertices->push_back( (*vertices)[RV(4)] );
		realVertices->push_back( (*vertices)[RV(6)] );
		realVertices->push_back( (*vertices)[RV(2)] );
		realTexcoords->push_back( (*texcoords)[RT(0)] );
		realTexcoords->push_back( (*texcoords)[RT(2)] );
		realTexcoords->push_back( (*texcoords)[RT(3)] );
		realTexcoords->push_back( (*texcoords)[RT(1)] );
		for (int j = 0; j < 8; j++)
			quadsTopBot->push_back( arrayPlace++ );
	}

	if (!isCircle) {
		int tc = (divisions * 2);
		// start face
		realVertices->push_back( (*vertices)[0] );
		realVertices->push_back( (*vertices)[2] );
		realVertices->push_back( (*vertices)[3] );
		realVertices->push_back( (*vertices)[1] );
		realTexcoords->push_back( (*texcoords)[0] );
		realTexcoords->push_back( (*texcoords)[tc] );
		realTexcoords->push_back( (*texcoords)[tc+1] );
		realTexcoords->push_back( (*texcoords)[1] );

		// end face
		int e = divisions * 4;
		realVertices->push_back( (*vertices)[e+2] );
		realVertices->push_back( (*vertices)[e] );
		realVertices->push_back( (*vertices)[e+1] );
		realVertices->push_back( (*vertices)[e+3] );
		realTexcoords->push_back( (*texcoords)[0] );
		realTexcoords->push_back( (*texcoords)[tc] );
		realTexcoords->push_back( (*texcoords)[tc+1] );
		realTexcoords->push_back( (*texcoords)[1] );
		for (int j = 0; j < 8; j++)
			quadsSide->push_back( arrayPlace++ );
	}

	sideMesh->setVertexArray( realVertices );
	sideMesh->setTexCoordArray( 0, realTexcoords );
	sideMesh->addPrimitiveSet( quadsSide );
	topbotMesh->setVertexArray( realVertices );
	topbotMesh->setTexCoordArray( 0, realTexcoords );
	topbotMesh->addPrimitiveSet( quadsTopBot );
}